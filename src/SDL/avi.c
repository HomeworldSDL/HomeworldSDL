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

/* Doesn't work */
//nova: this allows MSVC to compile this file
#ifndef _MSC_VER
#undef _WIN32
#endif

#ifdef _WIN32
#include <windows.h>
#include <vfw.h>
#include "wave.h"
#endif

#include "avi.h"
#include "utility.h"

#ifdef HW_ENABLE_MOVIES
 #include <ffmpeg/avformat.h>
 #include <ffmpeg/avcodec.h>
#endif

extern bool fullScreen;
extern void* ghMainWindow;
extern int MAIN_WindowWidth;
extern int MAIN_WindowHeight;
extern bool systemActive;

bool g_bMoreFrames;

int aviDonePlaying = 1;
int aviIsPlaying = 0;
int aviHasAudio = 0;

#ifdef HW_ENABLE_MOVIES
AVFormatContext *pFormatCtx    = NULL;
AVCodecContext  *pCodecCtx     = NULL;
AVStream        *streamPointer = NULL;
AVCodec         *pCodec        = NULL;
AVFrame         *pFrame        = NULL;
#endif

int aviMovieExpandFactor = 1;

#ifdef _WIN32	/* Disable AVI code outside of Windows. */

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

void aviReverseRGBA(ubyte * surf, int w, int h) {

    
#ifdef _WIN32
    ubyte line[4 * 640];  // This needs to hold a line of pixels 4bytes * width (640);
                          // sizeof(w) wouldn't give enough space. (I could be wrong though.)
#else
    ubyte line[4 * w];
#endif
    sdword y, top, bot;
    sdword pitch;

    pitch = 4*w;

    for (y = 0; y < (h/2); y++)
    {
        top = y;
        bot = (h -1) - y;

        memcpy(line, surf + pitch*top, pitch);
        memcpy(surf + pitch*top, surf + pitch*bot, pitch);
        memcpy(surf + pitch*bot, line, pitch);
    }
}

void aviRotateLeft( double c, int times) {
}

void aviARGBtoRGBA(ubyte * surf, int w, int h) {

/* Someone, somewhere is lying to me about the colour layout. The fix I'm going to do here
  Will be one Endian specific, and if anyone else is getting really strange colours out of the 
  animations, then start here. Especially if you get a Yellow background which is wat I've just 
  had.  Aunxx */
/* Update: OR more accurately they're telling the truth, and I make mistakes in my code. :( */

    int i, end;
    int temp;
    ubyte * cur;
   
    cur = surf;

    end = w*h*4;
    for (i =0; i< end ; i+=4){
        cur = surf +i;   
        temp = (int)cur ;
        memmove(cur+1, cur,3);
        cur +=3;
        cur = (ubyte *)temp;
    }
}

void aviStretchRGBA(ubyte * surf, int w, int h) {


    if (aviMovieExpandFactor != 1){

//    ubyte line[4*w];
//    sdword x,y, left, right;
//    sdword pitch;

//    pitch = 4;
//for (y=0; y< h ;y ++)
//{
//    for (x = w; x > 0; x--)
//    {
//        left = x;
//        right = (w -1) - x;

//        memcpy(line, surf + pitch*top, pitch);
//        memcpy(surf + pitch*top, surf + pitch*bot, pitch);
//        memcpy(surf + pitch*bot, line, pitch);
//    }

//}

    }
}

#ifdef HW_ENABLE_MOVIES

//void aviDisplayFrame( AVFrame *pFrameRGB )
void aviDisplayFrame( AVPicture *pFrameRGB, int w, int h )
{

    int x, y;

    x = (MAIN_WindowWidth  - w) / 2;
    y = (MAIN_WindowHeight  - h) / 2;

//  dbgMessagef("aviDisplayFrame: pFrameRGB=%x %x %x %x", pFrameRGB, pFrameRGB->data[0], pFrameRGB->data[1], pFrameRGB->data[2]);


    aviReverseRGBA( pFrameRGB->data[0], w, h );
    aviARGBtoRGBA( pFrameRGB->data[0], w, h );

    aviStretchRGBA( pFrameRGB->data[0], w, h );

    animAviSetup(TRUE);
    glRasterPos2i(x, y);

//    animAviSetup(FALSE);

    glDrawPixels (w,h, GL_RGBA, 0x8035, pFrameRGB->data[0]);

    animAviSetup(FALSE);


//  dbgMessagef("aviDisplayFrame: R=%s  G=%s  B=%s", testR, testG, testB);
//   dbgMessagef("aviDisplayFrame: R=%s", testR);


/*
    if (g_pbmi != NULL)
    {
        aviShowFrame(g_pbmi);
    }

	if(aviHasAudio == TRUE)
	{
		//give audio thread a break :)
		Sleep(0);
	}

*/
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
    Uint32 local_time, local_last_time, local_interval;
    SDL_Event e;
    char * buffer;
//    AVFrame *pFrameRGB;
    AVPicture *pPictureRGB;
    AVPicture PictureRGB;
    static AVPacket packet;

    pPictureRGB=&PictureRGB;

    numBytes=avpicture_get_size(PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);
#if AVI_VERBOSE_LEVEL >= 2
dbgMessagef("aviPlayLoop: numBytes= %d, width=%d height=%d", numBytes, pCodecCtx->width, pCodecCtx->height);
#endif

    buffer = av_malloc(numBytes );

    avpicture_fill(pPictureRGB, buffer, PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);

    while(av_read_frame(pFormatCtx, &packet)>=0) {

#if AVI_VERBOSE_LEVEL >= 9
dbgMessagef("aviPlayLoop: stream_index=%d, videoStream=%d", packet.stream_index, videoStream);
#endif

        avcodec_decode_video(pCodecCtx, pFrame, &frameFinished, packet.data, packet.size);

#if AVI_VERBOSE_LEVEL >= 100
dbgMessagef("aviPlayLoop: frameFinished=%d  packet.data=%x   packet.size=%d ", frameFinished, packet.data, packet.size);
#endif
// dbgMessagef("aviPlayLoop: pFrame=%x %x %x %x", pFrame, pFrame->data[0], pFrame->data[1], pFrame->data[2]);

        if(frameFinished) {
            // Convert the image from its native format to RGB
            img_convert(pPictureRGB, PIX_FMT_RGB32,
                (AVPicture*)pFrame, pCodecCtx->pix_fmt, pCodecCtx->width,
                pCodecCtx->height);

            animAviDecode(frame);

            local_time= SDL_GetTicks();
            local_interval = local_time - local_last_time ;
            local_last_time = local_time ;
            if ((local_interval > 0) && (local_interval < 76)) {
                SDL_Delay(77 - local_interval);  //Closer...  :)
            }

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
        }
        av_free_packet(&packet);

    }

    // Clear Allocs

    av_free(buffer);

//    av_free(pPictureRGB);

#endif //  HW_ENABLE_MOVIES

    
}

int aviStart(char* filename)
{

#ifdef HW_ENABLE_MOVIES

    int i;
    int videoStream = -1;

    if(av_open_input_file(&pFormatCtx, filename, NULL, 0, NULL)!=0) {
        dbgMessagef("aviStart: Unable to open AVI: %s",filename);
        return FALSE;
    }

#if AVI_VERBOSE_LEVEL >= 2
dump_format(pFormatCtx, 0, filename, 0);
#endif

    for(i=0; i<pFormatCtx->nb_streams; i++) {
        streamPointer=pFormatCtx->streams[i];
//    }
    if(streamPointer->codec->codec_type==CODEC_TYPE_VIDEO) {

#if AVI_VERBOSE_LEVEL >= 3
dbgMessagef("aviStart: Found Video Stream= %d.", i);
#endif

        videoStream=i;
        i+=pFormatCtx->nb_streams; // get out of the loop!
    }
   }
    if(videoStream==-1) {
        dbgMessage("aviStart: No Video Stream found");
        return FALSE;
    }

    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

#if AVI_VERBOSE_LEVEL >= 2
dbgMessagef("aviStart: Codec required: %d.", pCodecCtx->codec_id);
dbgMessagef("aviStart: Pix Format: %d.", pCodecCtx->pix_fmt);
// dbgMessagef("Frame Rate: %d/%d", pCodecCtx->frame_rate, pCodecCtx->frame_rate_base);
#endif

// Find the decoder for the video stream

    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) {
        dbgMessage("Unable to find decoder.");
        return FALSE;
    }

#if AVI_VERBOSE_LEVEL >= 2
dbgMessagef("aviStart: Codec required: %s.", pCodec->name);
#endif

    if(pCodec->capabilities & CODEC_CAP_TRUNCATED) {
        dbgMessage("Problem with the codec dealing with truncated frames.");
        pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
    }

// Open codec
    if(avcodec_open(pCodecCtx, pCodec)<0) {
        dbgMessage("Unable to open Codec");
        return FALSE;
    }

    pFrame=avcodec_alloc_frame();


/*   I've Left this in for Reference. Remove it later?   Aunxx
    HRESULT Res;

    //initialize the AVI library
    AVIFileInit();

    Res = AVIStreamOpenFromFile(&g_VidStream, filename, streamtypeVIDEO, 0, OF_READ, NULL);
    if (!aviVerifyResult(Res))
    {
        return FALSE;
    }

    g_pFrame = AVIStreamGetFrameOpen(g_VidStream, NULL);
    if (g_pFrame == NULL)
    {
        return FALSE;
    }

    Res = AVIStreamInfo(g_VidStream, &g_VidStreamInfo, sizeof(AVISTREAMINFO));
    if (!aviVerifyResult(Res))
    {
        return FALSE;
    }

    //now grab the audio stream and its info
    Res = AVIStreamOpenFromFile(&g_AudStream, filename, streamtypeAUDIO, 0, OF_READ, NULL);
    if (!aviVerifyResult(Res))
    {
        return FALSE;
    }

    Res = AVIStreamInfo(g_AudStream, &g_AudStreamInfo, sizeof(AVISTREAMINFO));
    if (!aviVerifyResult(Res))
    {
        return FALSE;
    }

    //convert the "rate and scale" values into meaningful numbers
    g_FramesPerSec  = (double)g_VidStreamInfo.dwRate / (double)g_VidStreamInfo.dwScale;
    g_SamplesPerSec = (double)g_AudStreamInfo.dwRate / (double)g_AudStreamInfo.dwScale;

	//check for compressed audio
	if((g_SamplesPerSec == (double)WAVE_SAMPLERATE)
		&& (g_AudStreamInfo.dwRate == WAVE_SAMPLERATE*(WAVE_BITSAMPLE/8)*WAVE_NUMCHAN)
		&& (g_AudStreamInfo.dwScale == (WAVE_BITSAMPLE/8)*WAVE_NUMCHAN))
			aviHasAudio = TRUE;

    //init the frame and sample numbers

    g_dwCurrFrame  = 0;
    g_dwCurrSample = 0;
*/

#endif // HW_ENABLE_MOVIES
    //so good so far
    return TRUE;
}


int aviGetSamples(void* pBuf, long* pNumSamples, long nBufSize)
{
#ifdef _WIN32
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
    av_close_input_file(pFormatCtx);
#endif

}
	
void aviSetScreen(int w, int h){

    int xOfs, yOfs;

    xOfs = (MAIN_WindowWidth  - 640) / 2;
    yOfs = (MAIN_WindowHeight - 480) / 2;

dbgMessagef("aviSetScreen: xOfs=%d yOfs=%d", xOfs, yOfs);

    animAviSetup(TRUE);

    glRasterPos2i(xOfs, yOfs);

    animAviSetup(FALSE);

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


bool aviPlay(char* filename)
{
#if AVI_VERBOSE_LEVEL >= 2
dbgMessage("aviPlay:Entering");
#endif
    char  fullname[1024];
    char* dir;

//TODO  Include Windows file structure. 

	dir = getenv("HW_Data") ? getenv("HW_Data") : regDataEnvironment;
    if (dir == NULL)
    {
//        strcpy(fullname, "Movies\\");
        strcpy(fullname, "Movies/");
    }
    else
    {
        strcpy(fullname, dir);
//        strcat(fullname, "\\Movies\\");
//        strcat(fullname, "/Movies/");
    }
    strcat(fullname, filename);

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

	    g_bMoreFrames = TRUE;
	    aviIsPlaying = TRUE;
	    aviDonePlaying = FALSE;
	    aviPlayLoop();
    aviIsPlaying = FALSE;

// taskResumesAll();


    aviStop();
    return 1;
}

int aviInit()
{

#ifdef HW_ENABLE_MOVIES
    av_register_all();
    avcodec_init();
#endif

    return 1;

}


int aviCleanup()
{
#ifdef _WIN32
	//cleanup audio
	if(EndWave() < 0) return FALSE;

    return TRUE;
#else
    return 1;
#endif
}

int aviIntroPlay()
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
#ifdef _WIN32
                aviPlay("Movies\\sierra.avi");
#else
                aviPlay("Movies/sierra.avi");
#endif
//                intro++;
                break;
            case 2:
                /*binkPlay("Movies\\relicintro.bik", NULL, NULL, S_RGB555, TRUE, ANIM00_Relic);*/
#ifdef _WIN32
                aviPlay("Movies\\relicintro.avi");
#else
                aviPlay("Movies/relicintro.avi");
#endif
//                    intro++;
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
