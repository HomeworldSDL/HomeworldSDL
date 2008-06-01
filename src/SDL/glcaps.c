/*=============================================================================
        Name    : glcaps.c
        Purpose : determine the capabilities of the GL currently in use

        Created 19/06/1998 by khent
        Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/


#include <string.h>
#include "glcaps.h"
#include "glext.h"
#include "Debug.h"
#include "texreg.h"
#include "StringSupport.h"
#include "devstats.h"
#include "main.h"
#include "render.h"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

/*-----------------------------------------------------------------------------
    data
-----------------------------------------------------------------------------*/

extern unsigned int gDevcaps;
extern unsigned int gDevcaps2;

char const* GLC_VENDOR;
char const* GLC_RENDERER;
char const* GLC_EXTENSIONS;

unsigned int RGL;
unsigned int RGLtype;

static sdword glCapTexFormat[] =
{
    GL_RGB,     1,
    GL_RGBA,    1,
    GL_RGBA16,  1,
    0, 0,
};

static bool glCapVertexArray       = FALSE;
static bool glCapPointSmooth       = FALSE;
static bool glCapLineSmooth        = FALSE;
static bool glCapPointSize         = FALSE;
static bool glCapDoubleBuffer      = FALSE;
static bool glCapSwapFriendly      = FALSE;
static bool glRescaleNormal        = FALSE;
static bool glPalettedTexture      = FALSE;
static bool glSharedTexturePalette = FALSE;
static bool glLitTexturePalette    = FALSE;
static bool glCompiledVertexArrays = FALSE;
static bool glClippingHint         = FALSE;

bool glNT = FALSE;
bool gl95 = FALSE;

GLenum glCapDepthFunc;

int NULL_rglINT(void)
{
#if defined (_MSC_VER)
    __asm int 3 ;
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ ( "int $3\n\t" );
#endif
    return 0;
}

void NULL_rglVOID(void)
{
#if defined (_MSC_VER)
    __asm int 3 ;
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ ( "int $3\n\t" );
#endif
}

char* NULL_rglPCHAR(void)
{
#if defined (_MSC_VER)
    __asm int 3 ;
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ ( "int $3\n\t" );
#endif
    return NULL;
}

#define RGLINT { return NULL_rglINT(); }
#define RGLVOID { NULL_rglVOID(); }
#define RGLBYTE { return (unsigned char)NULL_rglINT(); }
#define RGLUINT { return (unsigned int)NULL_rglINT(); }
#define RGLPCHAR { return NULL_rglPCHAR(); }
#define RGLPVOID { return (void*)NULL_rglPCHAR(); }

int (*rglFeature)(unsigned int feature);
int NULL_rglFeature(unsigned int feature) RGLINT

void (*rglSpecExp)(GLint index, GLfloat exp);
void NULL_rglSpecExp(GLint index, GLfloat exp) RGLVOID

void (*rglLightingAdjust)(GLfloat adj);
void NULL_rglLightingAdjust(GLfloat adj) RGLVOID

void (*rglSaveCursorUnder)(GLubyte* data, GLsizei width, GLsizei height, GLint x, GLint y);
void NULL_rglSaveCursorUnder(GLubyte* data, GLsizei width, GLsizei height, GLint x, GLint y) RGLVOID

void (*rglRestoreCursorUnder)(GLubyte* data, GLsizei width, GLsizei height, GLint x, GLint y);
void NULL_rglRestoreCursorUnder(GLubyte* data, GLsizei width, GLsizei height, GLint x, GLint y) RGLVOID

unsigned char (*rglIsFast)(unsigned int feature);
unsigned char NULL_rglIsFast(unsigned int feature) RGLBYTE

unsigned char (*rglCreateWindow)(GLint ihwnd, GLint width, GLint depth);
unsigned char NULL_rglCreateWindow(GLint ihwnd, GLint width, GLint depth) RGLBYTE

void (*rglDeleteWindow)(GLint);
void NULL_rglDeleteWindow(GLint a) RGLVOID

unsigned char (*rglIsClipped)(GLfloat*, GLfloat, GLfloat, GLfloat);
unsigned char NULL_rglIsClipped(GLfloat* a, GLfloat b, GLfloat c, GLfloat d) RGLBYTE

GLuint (*rglNumPolys)(void);
GLuint NULL_rglNumPolys(void) RGLUINT

GLuint (*rglCulledPolys)(void);
GLuint NULL_rglCulledPolys(void) RGLUINT

void (*rglBackground)(GLubyte* pixels);
void NULL_rglBackground(GLubyte* pixels) RGLVOID

void (*rglSetAllocs)(MemAllocFunc allocFunc, MemFreeFunc freeFunc);
void NULL_rglSetAllocs(MemAllocFunc allocFunc, MemFreeFunc freeFunc) RGLVOID

void (*rglSuperClear)(void);
void NULL_rglSuperClear(void) RGLVOID

void (*rglEnable)(GLint);
void NULL_rglEnable(GLint a) RGLVOID

void (*rglDisable)(GLint);
void NULL_rglDisable(GLint a) RGLVOID

void (*rglListSpec)(GLint pname, GLint param, GLint n, GLenum format);
void NULL_rglListSpec(GLint pname, GLint param, GLint n, GLenum format) RGLVOID

void (*rglList)(GLint pname, GLvoid const* list);
void NULL_rglList(GLint pname, GLvoid const* list) RGLVOID

void (*rglNormal)(GLint param);
void NULL_rglNormal(GLint param) RGLVOID

void (*rglTriangle)(GLint iPoly);
void NULL_rglTriangle(GLint iPoly) RGLVOID

void (*rglTexturedTriangle)(GLint iPoly);
void NULL_rglTexturedTriangle(GLint iPoly) RGLVOID

void (*rglSmoothTriangle)(GLint iPoly);
void NULL_rglSmoothTriangle(GLint iPoly) RGLVOID

void (*rglSmoothTexturedTriangle)(GLint iPoly);
void NULL_rglSmoothTexturedTriangle(GLint iPoly) RGLVOID

void (*rglMeshRender)(GLint n, void (*callback)(GLint material), GLint* meshPolyMode);
void NULL_rglMeshRender(GLint n, void (*callback)(GLint material), GLint* meshPolyMode) RGLVOID

void (*rglSelectDevice)(char* name, char* data);
void NULL_rglSelectDevice(char* name, char* data) RGLVOID

void* (*rglGetFramebuffer)(GLint* pitch);
void* NULL_rglGetFramebuffer(GLint* pitch) RGLPVOID

void (*rglDrawPitchedPixels)(GLint x0, GLint y0,
                             GLint x1, GLint y1,
                             GLsizei width, GLsizei height, GLsizei pitch,
                             GLvoid const* pixels);
void NULL_rglDrawPitchedPixels(GLint x0, GLint y0, GLint x1, GLint y1,
                               GLsizei width, GLsizei height, GLsizei pitch,
                               GLvoid const* pixels) RGLVOID

/*-----------------------------------------------------------------------------
    code
-----------------------------------------------------------------------------*/

typedef int (*RGLFEATUREPROC)(unsigned int);
typedef void (*RGLSPECEXPPROC)(GLint, GLfloat);
typedef void (*RGLLIGHTINGADJUSTPROC)(GLfloat);
typedef void (*RGLSAVECURSORUNDERPROC)(GLubyte*, GLsizei, GLsizei, GLint, GLint);
typedef void (*RGLRESTORECURSORUNDERPROC)(GLubyte*, GLsizei, GLsizei, GLint, GLint);
typedef unsigned char (*RGLISFASTPROC)(unsigned int);
typedef unsigned char (*RGLCREATEWINDOWPROC)(GLint, GLint, GLint);
typedef void (*RGLDELETEWINDOWPROC)(GLint);
typedef unsigned char (*RGLISCLIPPEDPROC)(GLfloat*, GLfloat, GLfloat, GLfloat);
typedef GLuint (*RGLNUMPOLYSPROC)(void);
typedef GLuint (*RGLCULLEDPOLYSPROC)(void);
typedef void (*RGLBACKGROUNDPROC)(GLubyte*);
typedef void (*RGLSETALLOCSPROC)(MemAllocFunc, MemFreeFunc);
typedef void (*RGLSUPERCLEARPROC)(void);
typedef void (*RGLENABLEPROC)(GLint);
typedef void (*RGLDISABLEPROC)(GLint);
typedef void (*RGLLISTSPECPROC)(GLint, GLint, GLint, GLenum);
typedef void (*RGLLISTPROC)(GLint, GLvoid const*);
typedef void (*RGLNORMALPROC)(GLint);
typedef void (*RGLTRIANGLEPROC)(GLint);
typedef void (*RGLTEXTUREDTRIANGLEPROC)(GLint);
typedef void (*RGLSMOOTHTRIANGLEPROC)(GLint);
typedef void (*RGLSMOOTHTEXTUREDTRIANGLEPROC)(GLint);
typedef void (*RGLMESHRENDERPROC)(GLint, void (*)(GLint), GLint*);
typedef void (*RGLSELECTDEVICEPROC)(char*, char*);
typedef void* (*RGLGETFRAMEBUFFERPROC)(GLint*);
typedef void (*RGLDRAWPITCHEDPIXELSPROC)(GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLvoid const*);


/*-----------------------------------------------------------------------------
    Name        : glCapGetRGLAddresses
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void glCapGetRGLAddresses()
{
    rglFeature = (RGLFEATUREPROC)rwglGetProcAddress("rglFeature");
    rglSpecExp = (RGLSPECEXPPROC)rwglGetProcAddress("rglSpecExp");
    rglLightingAdjust = (RGLLIGHTINGADJUSTPROC)rwglGetProcAddress("rglLightingAdjust");
    rglSaveCursorUnder = (RGLSAVECURSORUNDERPROC)rwglGetProcAddress("rglSaveCursorUnder");
    rglRestoreCursorUnder = (RGLRESTORECURSORUNDERPROC)rwglGetProcAddress("rglRestoreCursorUnder");
    rglIsFast = (RGLISFASTPROC)rwglGetProcAddress("rglIsFast");
    rglCreateWindow = (RGLCREATEWINDOWPROC)rwglGetProcAddress("rglCreateWindow");
    rglDeleteWindow = (RGLDELETEWINDOWPROC)rwglGetProcAddress("rglDeleteWindow");
    rglIsClipped = (RGLISCLIPPEDPROC)rwglGetProcAddress("rglIsClipped");
    rglNumPolys = (RGLNUMPOLYSPROC)rwglGetProcAddress("rglNumPolys");
    rglCulledPolys = (RGLCULLEDPOLYSPROC)rwglGetProcAddress("rglCulledPolys");
    rglBackground = (RGLBACKGROUNDPROC)rwglGetProcAddress("rglBackground");
    rglSetAllocs = (RGLSETALLOCSPROC)rwglGetProcAddress("rglSetAllocs");
    rglSuperClear = (RGLSUPERCLEARPROC)rwglGetProcAddress("rglSuperClear");
    rglEnable = (RGLENABLEPROC)rwglGetProcAddress("rglEnable");
    rglDisable = (RGLDISABLEPROC)rwglGetProcAddress("rglDisable");

    rglListSpec = (RGLLISTSPECPROC)rwglGetProcAddress("rglListSpec");
    rglList = (RGLLISTPROC)rwglGetProcAddress("rglList");
    rglNormal = (RGLNORMALPROC)rwglGetProcAddress("rglNormal");
    rglTriangle = (RGLTRIANGLEPROC)rwglGetProcAddress("rglTriangle");
    rglTexturedTriangle = (RGLTEXTUREDTRIANGLEPROC)rwglGetProcAddress("rglTexturedTriangle");
    rglSmoothTriangle = (RGLSMOOTHTRIANGLEPROC)rwglGetProcAddress("rglSmoothTriangle");
    rglSmoothTexturedTriangle = (RGLSMOOTHTEXTUREDTRIANGLEPROC)rwglGetProcAddress("rglSmoothTexturedTriangle");
    rglMeshRender = (RGLMESHRENDERPROC)rwglGetProcAddress("rglMeshRender");
    rglSelectDevice = (RGLSELECTDEVICEPROC)rwglGetProcAddress("rglSelectDevice");

    rglGetFramebuffer = (RGLGETFRAMEBUFFERPROC)rwglGetProcAddress("rglGetFramebuffer");
    rglDrawPitchedPixels = (RGLDRAWPITCHEDPIXELSPROC)rwglGetProcAddress("rglDrawPitchedPixels");
}

/*-----------------------------------------------------------------------------
    Name        : glCapResetRGLAddresses
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void glCapResetRGLAddresses(void)
{
    RGL = 0;

    rglFeature = NULL_rglFeature;
    rglSpecExp = NULL_rglSpecExp;
    rglLightingAdjust = NULL_rglLightingAdjust;
    rglSaveCursorUnder = NULL_rglSaveCursorUnder;
    rglRestoreCursorUnder = NULL_rglRestoreCursorUnder;
    rglIsFast = NULL_rglIsFast;
    rglCreateWindow = NULL_rglCreateWindow;
    rglDeleteWindow = NULL_rglDeleteWindow;
    rglIsClipped = NULL_rglIsClipped;
    rglNumPolys = NULL_rglNumPolys;
    rglCulledPolys = NULL_rglCulledPolys;
    rglBackground = NULL_rglBackground;
    rglSetAllocs = NULL_rglSetAllocs;
    rglSuperClear = NULL_rglSuperClear;
    rglEnable = NULL_rglEnable;
    rglDisable = NULL_rglDisable;

    rglListSpec = NULL_rglListSpec;
    rglList = NULL_rglList;
    rglNormal = NULL_rglNormal;
    rglTriangle = NULL_rglTriangle;
    rglTexturedTriangle = NULL_rglTexturedTriangle;
    rglSmoothTriangle = NULL_rglSmoothTriangle;
    rglSmoothTexturedTriangle = NULL_rglSmoothTexturedTriangle;
    rglMeshRender = NULL_rglMeshRender;
    rglSelectDevice = NULL_rglSelectDevice;

    rglGetFramebuffer = NULL_rglGetFramebuffer;
    rglDrawPitchedPixels = NULL_rglDrawPitchedPixels;
}

/*-----------------------------------------------------------------------------
    Name        : glCapFastFeature
    Description : "decides" whether a feature of the GL (limited subset) is "fast"
    Inputs      : feature - the GL capability (or "feature") to examine
    Outputs     :
    Return      : 0 (FALSE) or !0 (TRUE)
----------------------------------------------------------------------------*/
bool glCapFastFeature(GLenum feature)
{
    switch (feature)
    {
    case GL_BLEND:
        return RGL ? rglIsFast(RGL_FEATURE_BLEND) : TRUE;

    case GL_SRC_ALPHA_SATURATE:
        return RGL ? rglIsFast(RGL_FEATURE_BLEND) : FALSE;

    case GL_LINE_STIPPLE:
        return RGL ? TRUE : FALSE;

    default:
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
    Name        : glCapFeatureExists
    Description : "decides" whether a feature of the GL exists
    Inputs      : feature - the GL capability (or "feature") to examine
    Outputs     :
    Return      : 0 (FALSE) or !0 (TRUE)
----------------------------------------------------------------------------*/
bool glCapFeatureExists(GLenum feature)
{
    switch (feature)
    {
    case GL_VERTEX_ARRAY:
        return glCapVertexArray;

    case GL_SRC_ALPHA_SATURATE:
        return glCapFastFeature(feature);

    case GL_DOUBLEBUFFER:
        return glCapDoubleBuffer;

    case GL_SWAPFRIENDLY:
        return glCapSwapFriendly;

    case GL_RESCALE_NORMAL:
        return glRescaleNormal;

    case GL_COLOR_TABLE_FORMAT_EXT:
        return glPalettedTexture;

    case GL_SHARED_TEXTURE_PALETTE_EXT:
        return RGL ? rglFeature(GL_SHARED_TEXTURE_PALETTE_EXT) : glSharedTexturePalette;

    case GL_LIT_TEXTURE_PALETTE_EXT:
        return glLitTexturePalette;

    case GL_COMPILED_ARRAYS_EXT:
        return glCompiledVertexArrays;

    case GL_CLIP_VOLUME_CLIPPING_HINT_EXT:
        return glClippingHint;

    case GL_POINT_SMOOTH:
        return glCapPointSmooth;

    case GL_LINE_SMOOTH:
        return glCapLineSmooth;

    case RGL_BROKEN_MIXED_DEPTHTEST:
        return FALSE;

    case RGL_COLOROP_ADD:
        return RGL ? rglFeature(RGL_COLOROP_ADD) : FALSE;

    case GL_POINT_SIZE:
        return glCapPointSize;

    case GL_SCISSOR_TEST:
        return RGLtype;

    case GL_COLOR_CLEAR_VALUE:
        if (bitTest(gDevcaps, DEVSTAT_NO_BGCOLOUR))
        return FALSE;

    default:
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
    Name        : glCapTexSupport
    Description : determines whether the GL supports a given texture format
    Inputs      : format - the format to query
    Outputs     :
    Return      : TRUE or FALSE (!0 or 0)
----------------------------------------------------------------------------*/
bool glCapTexSupport(GLenum format)
{
    sdword i;

    for (i = 0;; i += 2)
    {
        if (glCapTexFormat[i] == 0)
        {
            break;
        }
        if (glCapTexFormat[i] == format)
        {
            return glCapTexFormat[i+1];
        }
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
    Name        : glCapNumBuffers
    Description : return the number of active buffers for swapfriendly-ness
    Inputs      :
    Outputs     :
    Return      : number of buffers in use for page flipping
----------------------------------------------------------------------------*/
sdword glCapNumBuffers(void)
{
    extern bool mainDoubleIsTriple;
//    if ((RGLtype == GLtype) && bitTest(gDevcaps, DEVSTAT_GL_TRIPLE))
    if (RGLtype == GLtype)
    {
        return 3;
    }
    else
    {
        return mainDoubleIsTriple ? 3 : 2;
    }
}

/*-----------------------------------------------------------------------------
    Name        : glCapLoadOpenGL
    Description :
    Inputs      : dllName - name of the .DLL
    Outputs     :
    Return      : 0 (failure) or !0 (success)
----------------------------------------------------------------------------*/
bool glCapLoadOpenGL(char* dllName)
{
    bool rval;

    rval = glDLLGetProcs(dllName);
#ifdef _WIN32
			if (rval && (strcasecmp(dllName, "opengl32.dll") == 0))
//    if (rval && (strcasecmp(dllName, "librgl.dll") == 0))
#else
    if (rval && (strcasecmp(dllName, "librgl.so") == 0))
#endif
    {
        glCapGetRGLAddresses();
    }

    return rval;
}

/*-----------------------------------------------------------------------------
    Name        : haveExtension
    Description : determines whether a given GL extension is supported
    Inputs      : extName - full name of the extension
                  extString - GL_EXTENSIONS string
    Outputs     :
    Return      : TRUE (have) or FALSE (have not)
----------------------------------------------------------------------------*/
static bool haveExtension(char* extName, char const* extString)
{
    char* p;
    char* end;
    sdword extNameLen;

    p = (char*)extString;

    extNameLen = strlen(extName);
    end = p + strlen(p);

    while (p < end)
    {
        sdword n = strcspn(p, " ");
        if ((extNameLen == n) && (strncmp(extName, p, n) == 0))
        {
            return TRUE;
        }
        p += (n + 1);
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
    Name        : glCapNT
    Description : determines whether running NT <= 4
    Inputs      :
    Outputs     :
    Return      : TRUE or FALSE (NT <= 4 or not)
----------------------------------------------------------------------------*/
bool glCapNT(void)
{
#ifdef _WIN32
    OSVERSIONINFO osVer;

    osVer.dwOSVersionInfoSize = sizeof(osVer);
    if (!GetVersionEx(&osVer))
    {
        return TRUE;
    }
    if (osVer.dwPlatformId == VER_PLATFORM_WIN32_NT &&
        osVer.dwMajorVersion <= 4)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;
#endif
}

/*-----------------------------------------------------------------------------
    Name        : glCap95
    Description : determines whether running 95, or something less than "Windows 4.10"
    Inputs      :
    Outputs     :
    Return      : TRUE or FALSE ('95 or not)
----------------------------------------------------------------------------*/
bool glCap95(void)
{
#ifdef _WIN32
    OSVERSIONINFO osVer;

    osVer.dwOSVersionInfoSize = sizeof(osVer);
    if (!GetVersionEx(&osVer))
    {
        return TRUE;
    }
    if (osVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS &&
        osVer.dwMinorVersion < 10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;
#endif
}

/*-----------------------------------------------------------------------------
    Name        : glCapValidGL
    Description : check to see if GetString is returning something sane,
                  to determine whether the GL is in a sane state
    Inputs      :
    Outputs     :
    Return      : TRUE or FALSE
----------------------------------------------------------------------------*/
bool glCapValidGL(void)
{
    GLC_RENDERER = (char const*)glGetString(GL_RENDERER);
    if (GLC_RENDERER == NULL)
    {
        dbgMessage("glCapValidGL: !INVALID!");
        return FALSE;
    }
    else
    {
        dbgMessagef("glCapValidGL: VALID '%s'", GLC_RENDERER);
        return TRUE;
    }
}

/*-----------------------------------------------------------------------------
    Name        : glCapStartup
    Description : actually queries the GL to test support for the varying texture formats
                  and extensions, and whatnot
    Inputs      :
    Outputs     : glCapTexFormat[] is modified
    Return      :
----------------------------------------------------------------------------*/
char GENERIC_OPENGL_RENDERER[128];
void glCapStartup(void)
{
    sdword i;
    GLubyte data[4*16*16];
    GLint param;
    char* str;
    extern bool mainFastFrontend;
    extern bool mainNoPalettes;

#ifdef _MACOSX_FIX_ME
	if( !rndSmallInit( NULL, TRUE ) )
	{
		fprintf( stderr, "Cannot initialize OpenGL\n" );
		return;
	}
#endif

    GLC_EXTENSIONS = (char const*)glGetString(GL_EXTENSIONS);
    GLC_RENDERER   = (char const*)glGetString(GL_RENDERER);
    GLC_VENDOR     = (char const*)glGetString(GL_VENDOR);

    if (GLC_EXTENSIONS == NULL)
    {
        RGL = 0;
        RGLtype = GLtype;
        return;
    }

    //obtain Microsoft generic rasterizer RENDERER string
    str = strGetString(strGDIGenericRenderer);
    if (str == NULL)
    {
        strcpy(GENERIC_OPENGL_RENDERER, "GDI Generic");
    }
    else
    {
        strncpy(GENERIC_OPENGL_RENDERER, str, 127);
    }
    GENERIC_OPENGL_RENDERER[127] = '\0';

    //see if we're using rGL or OpenGL
    if (haveExtension("GL_RGL_rgl_feature", GLC_EXTENSIONS))
    {
        RGL = TRUE;
        glCapGetRGLAddresses();
    }
    else
    {
        RGL = 0;
        RGLtype = GLtype;
        glCapResetRGLAddresses();
    }

    //determine texture format support
    for (i = 0;; i += 2)
    {
        if (glCapTexFormat[i] == 0)
        {
            break;
        }
        if (RGL)
        {
            if ((RGLtype == SWtype) && (glCapTexFormat[i] == GL_RGBA16))
            {
                glCapTexFormat[i+1] = 0;
            }
            else
            {
                glCapTexFormat[i+1] = 1;
            }
        }
        else
        {
            glTexImage2D(GL_PROXY_TEXTURE_2D, 0, glCapTexFormat[i], 16, 16,
                         0, glCapTexFormat[i], GL_UNSIGNED_BYTE, data);
            glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &param);
            if (param == glCapTexFormat[i])
            {
                glCapTexFormat[i+1] = 1;
            }
            else
            {
                glCapTexFormat[i+1] = 0;
            }
        }
    }

    //determine supported extensions
    glRescaleNormal = haveExtension("GL_EXT_rescale_normal", GLC_EXTENSIONS);
    glPalettedTexture = haveExtension("GL_EXT_paletted_texture", GLC_EXTENSIONS);
    glSharedTexturePalette = haveExtension("GL_EXT_shared_texture_palette", GLC_EXTENSIONS);
    if (mainNoPalettes || bitTest(gDevcaps, DEVSTAT_NOPAL))
    {
        //no palettes if globally disabled, or for this device
        trNoPalettes = TRUE;
    }
    else
    {
        if (glPalettedTexture && glSharedTexturePalette)
        {
            //palette extensions detected
            trNoPalettes = FALSE;
        }
        else
        {
            //no palette extensions available
            trNoPalettes = TRUE;
        }
    }

    //palettes are not "safe"
    if (mainSafeGL && (RGLtype == GLtype))
    {
        trNoPalettes = TRUE;
    }

    //determine DrawArrays & DrawElements support
    if (strstr(GLC_RENDERER, "Savage") != NULL)
    {
        //Savage 3/4 crash on interleaved vertex arrays
        glCapVertexArray = FALSE;
    }
    else
    {
        //everything else seems ok
        glCapVertexArray = TRUE;
    }

    if (mainSafeGL && (RGLtype == GLtype))
    {
        //vertex array extensions are not "safe"
        glCapVertexArray = FALSE;
    }

    glCompiledVertexArrays = haveExtension("GL_EXT_compiled_vertex_array", GLC_EXTENSIONS);
    glClippingHint = haveExtension("GL_EXT_clip_volume_hint", GLC_EXTENSIONS);
    glLitTexturePalette = haveExtension("GL_RGL_lit_texture_palette", GLC_EXTENSIONS);

    if (mainSafeGL && (RGLtype == GLtype))
    {
        //compiled vertex arrays are not "safe"
        glCompiledVertexArrays = FALSE;
    }

    glCapSwapFriendly = FALSE;
    glCapPointSize = TRUE;

    if (RGL)
    {
        glCapDoubleBuffer = TRUE;
        if (RGLtype != SWtype)
        {
            glLitTexturePalette = FALSE;
        }

        //recommended depthbuffer function
        glCapDepthFunc = GL_LESS;
    }
    else
    {
        //determine double buffering support
        glGetIntegerv(GL_DOUBLEBUFFER, &param);
        glCapDoubleBuffer = param ? TRUE : FALSE;

        //recommended depthbuffer function
        glCapDepthFunc = GL_LEQUAL;
    }

    //enable/disable linesmoothing as per devcaps et al
    glCapLineSmooth = TRUE;

    //enable/disable pointsmoothing as per devcaps et al
    glCapPointSmooth = !RGL;
    if (!glCapLineSmooth)
    {
        //no pointsmoothing if no linesmoothing
        glCapPointSmooth = FALSE;
    }
    else if (strstr(GLC_RENDERER, "Savage") != NULL)
    {
        //Savage 3/4 doesn't want pointsmoothing
        glCapPointSmooth = FALSE;
    }
    else if (bitTest(gDevcaps, DEVSTAT_NO_POINTSMOOTH))
    {
        //check devcaps bit
        glCapPointSmooth = FALSE;
    }

    if (mainSafeGL && (RGLtype == GLtype))
    {
        //antialiasing is not "safe"
        glCapLineSmooth = FALSE;
        glCapPointSmooth = FALSE;
    }

    //enable/disable fast frontend as per devcaps
    if (mainFastFrontend)
    {
        glCapSwapFriendly = TRUE;
    }
    else
    {
        glCapSwapFriendly = FALSE;
    }

    if (RGLtype == SWtype)
    {
        //software is always swapfriendly
        glCapSwapFriendly = TRUE;
    }
    else
    {
        if (bitTest(gDevcaps, DEVSTAT_NOFFE) ||
            bitTest(gDevcaps, DEVSTAT_NOFFE_GL))
        {
            //disabled as per devcaps
            glCapSwapFriendly = FALSE;
        }
    }
}
