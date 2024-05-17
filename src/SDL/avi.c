/*=============================================================================
    Name    : avi.c
    Purpose : routines for playing AVI files

    Created 1/9/1999 by jdorie, khent, jjoire
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/
/* TC 2003-10-01:
 * Ever so silently, we replace AVI functions with dummy functions when
 * compiling on platforms other than Windows.  Shh...don't tell the game what
 * we're doing...
 */

#include "Animatic.h"
#include "avi.h"
#include "Debug.h"
#include "File.h"
#include "glinc.h"
#include "NIS.h"
#include "Subtitle.h"
#include "Tutor.h"
#include "utility.h"
#include "Universe.h"
#include "render.h"
#include "SoundEvent.h"

//nova: this allows MSVC to compile this file
#ifdef _WIN32
 	#define sleep(x) _sleep((x) * 1000)
#endif

#ifdef HW_ENABLE_MOVIES_WIN32
    #include <windows.h>
    #include <Vfw.h>
    #include "wave.h"
#endif

#ifdef HW_ENABLE_MOVIES
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #define ALIGN 32
#endif


/*=============================================================================
    Switches
=============================================================================*/


extern bool32 fullScreen;
extern void* ghMainWindow;
extern int MAIN_WindowWidth;
extern int MAIN_WindowHeight;
extern bool32 systemActive;

bool32 g_bMoreFrames;

int aviDonePlaying = 1;
int aviIsPlaying = 0;
int aviHasAudio = 0;

// Set to 1 if you want to watch the Intros.
udword aviPlayIntros = 0;

#ifdef HW_ENABLE_MOVIES
AVFormatContext *pFormatCtx    = NULL;
AVCodecContext  *pCodecCtx     = NULL;
AVStream        *streamPointer = NULL;
AVCodec         *pCodec        = NULL;
AVFrame         *pFrame        = NULL;
AVDictionary    **opts         = NULL;
AVCodecParameters *pCodecpar   = NULL;
AVCodecParserContext *parser   = NULL;
#endif

int aviMovieExpandFactor = 1;

#ifdef HW_ENABLE_MOVIES_WIN32	/* Disable AVI code outside of Windows. */

PAVISTREAM    g_VidStream;      //the AVI video stream
AVISTREAMINFO g_VidStreamInfo;  //info about the AVI stream

PAVISTREAM    g_AudStream;      //the AVI audio stream
AVISTREAMINFO g_AudStreamInfo;  //info about the AVI stream

long          g_dwCurrFrame;    //current frame of the AVI
long          g_dwCurrSample;   //current sample of the AVI
PGETFRAME     g_pFrame;         //an object to hold info about the video frame when we get it

double g_FramesPerSec;
double g_SamplesPerSec;

HBITMAP g_hBitmap = 0;
WORD*   g_pBitmap = 0;

MMRESULT g_timerHandle = NULL;

int aviVerifyResult(HRESULT Result)
{
    return (Result == 0 ? 1 : 0);
}


BOOL aviGetNextFrame(BITMAPINFO** ppbmi)
{
    *ppbmi = (LPBITMAPINFO)AVIStreamGetFrame(g_pFrame, g_dwCurrFrame);
    g_dwCurrFrame++;

    //any more frames left?
    return !(g_dwCurrFrame >= g_VidStreamInfo.dwLength);
}

void aviResetStream(void)
{
    g_dwCurrFrame  = 0;
    g_dwCurrSample = 0;
}


void aviShowFrame(BITMAPINFO* pMap)
{
    HDC hdc, hdcTemp;
    int XbmSize, YbmSize;
    int xOffset, yOffset;

    XbmSize = pMap->bmiHeader.biWidth;
    YbmSize = pMap->bmiHeader.biHeight;

    hdc = GetDC(ghMainWindow);
    g_hBitmap = CreateDIBSection(hdc, pMap, DIB_RGB_COLORS, (VOID**)&g_pBitmap, NULL, NULL);
    SetDIBits(hdc, g_hBitmap, 0, YbmSize, (BYTE*)(&pMap->bmiColors), pMap, DIB_RGB_COLORS);

    hdcTemp = CreateCompatibleDC(hdc);
    SelectObject(hdcTemp, g_hBitmap);

    if (!fullScreen &&
        (XbmSize != MAIN_WindowWidth || YbmSize != MAIN_WindowHeight))
    {
        xOffset = (MAIN_WindowWidth  - XbmSize) / 2;
        yOffset = (MAIN_WindowHeight - YbmSize) / 2;
    }
    else
    {
        xOffset = 0;
        yOffset = 0;
    }
    BitBlt(hdc, xOffset, yOffset, XbmSize, YbmSize, hdcTemp, 0, 0, SRCCOPY);

    DeleteDC(hdcTemp);
    ReleaseDC(ghMainWindow, hdc);

    DeleteObject(g_hBitmap);
}

BITMAPINFO* g_pbmi = NULL;

void CALLBACK aviTimeProc(UINT uid, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (g_bMoreFrames)
	{
		g_bMoreFrames = aviGetNextFrame(&g_pbmi);
	}
	else
	{
		aviDonePlaying = TRUE;
	}
}

#endif	/* _WIN32 */

#ifdef HW_ENABLE_MOVIES
static GLint strtex;
static int texinit = 0;

static void Draw_Stretch (int x, int y, int w, int h, int cols, int rows, char *data)
{
    GLsizei tex_width = 1;
    GLsizei tex_height = 1;

    while (tex_width < cols) tex_width <<= 1;
    while (tex_height < rows) tex_height <<= 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    if (!texinit) {
        //unsigned int i;
        unsigned char blank[tex_width*tex_height*3];
        memset(blank, 0, tex_width*tex_height*3);

        glGenTextures(1, &strtex);
        glBindTexture(GL_TEXTURE_2D, strtex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, blank);
        texinit = 1;
    } else {
        // In principle we don't need the else case but for some reason doing
        // the glTexImage2D call and immediately after the glTexSubImage2D call causes
        // a single frame flicker.
        // We loose the very first video frame we start to play.
        glBindTexture(GL_TEXTURE_2D, strtex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cols, rows, GL_RGB, GL_UNSIGNED_BYTE, data);

        glBegin (GL_QUADS);
        glTexCoord2f (0.0f, (GLfloat)rows/tex_height);
        glVertex2f (x, y);
        glTexCoord2f ((GLfloat)cols/tex_width, (GLfloat)rows/tex_height);
        glVertex2f (x+w, y);
        glTexCoord2f ((GLfloat)cols/tex_width, 0.0f);
        glVertex2f (x+w, y+h);
        glTexCoord2f (0.0f, 0.0f);
        glVertex2f (x, y+h);
        glEnd ();
    }
}

void aviDisplayFrame( AVFrame *pFrameRGB, int w, int h )
{
    animAviSetup(TRUE);

    // lets keep the aspect ratio of the video and draw it centered with max height or width
    float contentAspect = 1.85f; // actual video content aspect ratio without the letter boxing
    float videoAspect = (float)w / (float)h;
    float screenAspect = (float)MAIN_WindowWidth / (float)MAIN_WindowHeight;

    int imageHeight, imageWidth;

    if (screenAspect >= contentAspect)
    {
        // screen is wider than the video content
        // make use of full screen height by getting rid of the letter boxing and maintain the contents 1.85f aspect ratio
        float rescaleFactor = contentAspect / videoAspect;
        imageHeight = (int)(MAIN_WindowHeight * rescaleFactor);
        imageWidth = (int)(MAIN_WindowHeight * videoAspect * rescaleFactor);
    }
    else
    {
        // screen is taller than the video content, use full screen width
        imageHeight = (int)(MAIN_WindowWidth / videoAspect);
        imageWidth = (int)(MAIN_WindowWidth);
    }

    int x = (MAIN_WindowWidth - imageWidth) / 2;
    int y = (MAIN_WindowHeight - imageHeight) / 2;

    Draw_Stretch(x, y, imageWidth, imageHeight, w, h, pFrameRGB->data[0]);

    animAviSetup(FALSE);
}

#endif

void aviSubUpdate(void) {

    int index;

    for (index = 0; index < SUB_NumberRegions; index++)
    {
        if ( index == STR_LetterboxBar)
        {                                           //if this is the subtitle region
            tutDrawTextPointers(&subRegion[STR_LetterboxBar].rect);//draw any active pointers there may be
        }
        if (subRegion[index].bEnabled && subRegion[index].cardIndex > 0)
        {
            if (index == STR_NIS)
            {
                subTimeElapsed = &thisNisPlaying->timeElapsed;
            }
            else
            {
                subTimeElapsed = &universe.totaltimeelapsed;
            }

            // disable dropshadow for movie subtitles since they are somehow buggy
            // and it seems like they were supposed to be drawn differently
            // TODO: figure out why it is buggy
            
            subcard *card;
            int index2;
            for (index2 = 0, card = subRegion[index].card; index2 < subRegion[index].cardIndex; index2++, card++)
            {
                if (card->bDropShadow)
                {
                    card->bDropShadow = FALSE;
                }
            }

            subTitlesDraw(&subRegion[index]);
        }
    }

}

void aviPlayLoop()
{

#if AVI_VERBOSE_LEVEL >= 2
dbgMessage("aviPlayLoop:");
#endif

#ifdef HW_ENABLE_MOVIES

    int frame = 0;
    int numBytes;
    int frameFinished;
    int event_res = 0;
    Uint32 start_time;
    SDL_Event e;
    char * buffer;
//    AVFrame *pFrameRGB;
    AVFrame *pPictureRGB;
    AVFrame PictureRGB;
    static AVPacket packet;

    pPictureRGB=&PictureRGB;

    static struct SwsContext *img_convert_ctx = NULL;

    numBytes=av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, ALIGN);
#if AVI_VERBOSE_LEVEL >= 2
dbgMessagef("aviPlayLoop: numBytes= %d, width=%d height=%d", numBytes, pCodecCtx->width, pCodecCtx->height);
#endif

    buffer = av_malloc(numBytes );

    av_image_fill_arrays(pPictureRGB->data, pPictureRGB->linesize, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, ALIGN);

    if (img_convert_ctx == NULL){
        img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
        
    }

    start_time = SDL_GetTicks();

    while(av_read_frame(pFormatCtx, &packet)>=0) {

#if AVI_VERBOSE_LEVEL >= 9
dbgMessagef("aviPlayLoop: stream_index=%d, videoStream=%d", packet.stream_index, videoStream);
#endif

// avcodec_decode_video has been depreciated in preference for avcodec_decode_video2
//        avcodec_decode_video(pCodecCtx, pFrame, &frameFinished, packet.data, packet.size);
        //avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
        avcodec_send_packet(pCodecCtx, &packet);
        //avcodec_receive_frame(pCodecCtx, pFrame);

#if AVI_VERBOSE_LEVEL >= 100
dbgMessagef("aviPlayLoop: frameFinished=%d  packet.data=%x   packet.size=%d ", frameFinished, packet.data, packet.size);
#endif


        if(avcodec_receive_frame(pCodecCtx, pFrame)==0) {

            sws_scale(img_convert_ctx, pFrame->data,pFrame->linesize, 0, pCodecCtx->height, pPictureRGB->data,pPictureRGB->linesize);
            
            animAviDecode(frame);

            soundEventUpdate();
            speechEventUpdate();   //Keep this it works. :)
            rndClearToBlack();

            aviDisplayFrame(pPictureRGB, pCodecCtx->width,pCodecCtx->height);

            aviSubUpdate();

            rndFlush();

            SDL_PollEvent(&e);
            if(e.type == SDL_KEYDOWN){
                dbgMessage("Keyboard Event");
                break;
            }

            frame++;

            // Videos are played at 15 fps, try to keep framse in sync to that
            while (SDL_GetTicks() < (start_time + ((frame * 1000) / 15)))
                SDL_Delay(1);
        }
        av_packet_unref(&packet);

    }

#if AVI_VERBOSE_LEVEL >= 2
dbgMessagef("aviPlayLoop: play_time=%d ", SDL_GetTicks() - start_time);
#endif
    // Clear Allocs

    av_free(buffer);

//    av_free(pPictureRGB);

#endif //  HW_ENABLE_MOVIES

    
}

int aviStart(char* filename)
{

#ifdef HW_ENABLE_MOVIES

    int i = 0;
    int alignDoubleSet = 1;
    int ffmpegAlign = 1;
    int videoStream = -1;

// Add intelligence to identifying path and OS here  -- TODO

    if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0) {
        dbgMessagef("aviStart: Unable to open AVI: %s",filename);
        return FALSE;
    }

#if AVI_VERBOSE_LEVEL >= 2
    av_dump_format(pFormatCtx, 0, filename, 0);
#endif

#if AVI_VERBOSE_LEVEL >= 2
    dbgMessagef("sizeof  AVFormatContext = %d",sizeof(AVFormatContext));
#endif


    avformat_find_stream_info(pFormatCtx, NULL);

    videoStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &pCodec, 0);

#if AVI_VERBOSE_LEVEL >= 3
    dbgMessagef("aviStart: Found Video Stream= %d.", i);
#endif

    if(videoStream==-1) {
        dbgMessage("aviStart: No Video Stream found");
        return FALSE;
    }


/*
    pCodecpar = pFormatCtx->streams[0]->codecpar;


    pCodecpar->width=640;   //mpeg4 does not give this in bitstream
    pCodecpar->height=480;  //whatever that means
    pCodec=avcodec_find_decoder(pCodecpar->codec_id);
    parser = av_parser_init(pCodec->id);
    pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->width=640;
    pCodecCtx->height=480;
    pCodecCtx->pix_fmt=AV_PIX_FMT_YUV420P;  //manually set the pixel format from ffplay output
*/
    pCodecCtx = avcodec_alloc_context3(pCodec);

    int ret;
    char error[64];
    if( ( ret = avcodec_parameters_to_context( pCodecCtx, pFormatCtx->streams[videoStream]->codecpar ) ) < 0 )
	{
		av_strerror( ret, error, sizeof( error ) );
		dbgMessagef( "idCinematic: Failed to create video codec context from codec parameters with error: %s\n", error );
	}

#if AVI_VERBOSE_LEVEL >= 2
    dbgMessagef("aviStart: Codec required: %d.", pFormatCtx->streams[videoStream]->codecpar->codec_id);
    //dbgMessagef("aviStart: Pix Format: %d.", pFormatCtx->streams[videoStream]->codecpar->pix_fmt);
    //dbgMessagef("Frame Rate: %d/%d", pCodecCtx->frame_rate, pCodecCtx->frame_rate_base);
#endif

// Find the decoder for the video stream

    if(pCodec==NULL) {
        dbgMessage("Unable to find decoder.");
        return FALSE;
    }

#if AVI_VERBOSE_LEVEL >= 2
    dbgMessagef("aviStart: Codec required: %s.", pCodec->name);
#endif

/* this results in corrupted video */
/*    if(pCodec->capabilities & CODEC_CAP_TRUNCATED) {
        dbgMessage("Problem with the codec dealing with truncated frames.");
        pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
    }*/

// Open codec
    if( ( ret = avcodec_open2(pCodecCtx, pCodec, NULL) ) < 0 )
	{
		av_strerror( ret, error, sizeof( error ) );
		dbgMessagef( "Cannot open video decoder with error: %s\n", error );
		dbgMessage("Unable to open Codec");
        return FALSE;
	}

    pFrame=av_frame_alloc();    //what does this do?
                                //this is the last line in this function....


#endif // HW_ENABLE_MOVIES
    //so good so far
    return TRUE;
}


int aviGetSamples(void* pBuf, long* pNumSamples, long nBufSize)
{
#ifdef HW_ENABLE_MOVIES_WIN32
    HRESULT Res;
    long nSampRead;

	if(aviHasAudio == FALSE) return FALSE;

	if(aviIsPlaying == FALSE)
	{
		memset(pBuf,0,nBufSize);
		return TRUE;
	}

    Res = AVIStreamRead(g_AudStream, g_dwCurrSample, *pNumSamples, pBuf, nBufSize, NULL, &nSampRead);
    if (!aviVerifyResult(Res))
    {
        return FALSE;
    }

    *pNumSamples = nSampRead;
    g_dwCurrSample += nSampRead;

	return TRUE;
#else
    return 0;
#endif
}

void aviFileExit (void){

#ifdef HW_ENABLE_MOVIES
    avformat_close_input(&pFormatCtx);
#endif

}
	
void aviSetScreen(int w, int h){

    int xOfs, yOfs;

    xOfs = (MAIN_WindowWidth  - 640) / 2;
    yOfs = (MAIN_WindowHeight - 480) / 2;

dbgMessagef("aviSetScreen: xOfs=%d yOfs=%d", xOfs, yOfs);

}

int aviStop(void)
{
#ifdef HW_ENABLE_MOVIES
   
    SDL_Delay(500); // Give the audio time to stop

    avcodec_close(pCodecCtx); 
    aviFileExit();
#endif
    return 1;
}


bool32 aviPlay(char* filename)
{
#if AVI_VERBOSE_LEVEL >= 2
dbgMessage("aviPlay:Entering");
#endif
    char  fullname[1024];

//TODO  Include Windows file structure. 

    strcpy(fullname, filePathPrepend(filename, FF_HomeworldDataPath));

    //try Homeworld\Data\Movies first
    if (!aviStart(fullname))
    {
        //try current directory next
        if (!aviStart(filename))
        {
            return FALSE;
        }
    }

//taskFreezeAll();

//    aviSetScreen(pCodecCtx->width,pCodecCtx->height);

//    nisFadeToSet(a , 0, b);

    g_bMoreFrames  = TRUE;
    aviIsPlaying   = TRUE;
    aviDonePlaying = FALSE;
    aviPlayLoop();

    aviStop();
    aviIsPlaying = FALSE;
    
    return 1;
}

int aviInit()
{

//#ifdef HW_ENABLE_MOVIES
    //av_register_all(); //can be ignored in ffmpeg 4+
//#endif

    return 1;

}


int aviCleanup()
{
#ifdef HW_ENABLE_MOVIES_WIN32
	//cleanup audio
	if(EndWave() < 0) return FALSE;

    return TRUE;
#else
    return 1;
#endif
}

void aviIntroPlay()
{
    int intro;
    utilPlayingIntro = TRUE;

    for (intro = 0;intro < 4;intro ++) {

        switch (intro) {
            case 0:
                /*binkInit(-1);*/
                aviInit();
//                intro++;
                break;
            case 1:
                /*binkPlay("Movies\\sierra.bik", NULL, NULL, S_RGB555, TRUE, ANIM00_Sierra);*/
                if (aviPlayIntros) {
#ifdef _WIN32
                    aviPlay("Movies\\sierra.bik");
#else
                    aviPlay("Movies/sierra.bik");
#endif
//                intro++;
                }
                break;
            case 2:
                /*binkPlay("Movies\\relicintro.bik", NULL, NULL, S_RGB555, TRUE, ANIM00_Relic);*/
                if (aviPlayIntros) {
#ifdef _WIN32
                    aviPlay("Movies\\relicintro.bik");
#else
                    aviPlay("Movies/relicintro.bik");
#endif
//                    intro++;
                  }
                  break;
            case 3:
                /*binkCleanup();*/
                mainCleanupAfterVideo();
//                intro++;
                break;
//            case 4:
//                goto DONE_INTROS;
        }
    }
}
