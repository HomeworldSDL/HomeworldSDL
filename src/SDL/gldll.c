/*=============================================================================
    Name    : gldll.c
    Purpose : alternative to linking against OpenGL32.dll

    Created 12/14/1998 by khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifdef _MACOSX
    #include <CoreServices/CoreServices.h>
#endif

#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gldll.h"
#include "glcaps.h"
#include "Debug.h"
#include "devstats.h"
#include "main.h"

#if !defined(_MACOSX) && !defined(_WIN32)
    #include <dlfcn.h>
#endif

extern udword gDevcaps2;

#define WRAPPERS 0
#define BEGIN_WRAPPERS 0
#define TEXIMAGE_WRAPPERS 0
#define VERTEX_WRAPPERS 0
#define GENTEX_WRAPPERS 0
#define DELTEX_WRAPPERS 0

#ifdef _WIN32
#define GL_LIB_NAME( base )     (#base ".dll")
#else
#define GL_LIB_NAME( base )     ("lib" #base ".so")
#endif

#define _ERROR_WRAP( func ) ((func) != 0) ? DynalinkFailed : (DynalinkFailed |= 1)
#define _STRINGIZE_NAME(name) #name
#define _STRINGIZE_NAME0(name) _STRINGIZE_NAME(name)
#define _WGL_STRINGIZE_NAME( name ) _STRINGIZE_NAME0(wgl##name)
#define _GL_STRINGIZE_NAME( name) _STRINGIZE_NAME0(gl##name)
#define _DGL_STRINGIZE_NAME( name ) _STRINGIZE_NAME0(_gl##name)

#define DYNALINK_GL_FUNCTION( name )  _ERROR_WRAP((*(int (APIENTRY **)())&(gl##name))   = SDL_GL_GetProcAddress(_GL_STRINGIZE_NAME(name)))
#define DYNALINK_DGL_FUNCTION( name ) _ERROR_WRAP((*(int (APIENTRY **)())&(_gl##name))  = SDL_GL_GetProcAddress(_GL_STRINGIZE_NAME(name)))
#define DYNALINK_WGL_FUNCTION( name ) _ERROR_WRAP((*(int (APIENTRY **)())&(rwgl##name)) = SDL_GL_GetProcAddress(_WGL_STRINGIZE_NAME(name)))

#if WRAPPERS
static GLuint lastBound = 0;
#endif

#if TEXIMAGE_WRAPPERS
#define NUM_WATCHES 2048
static int watchedIndex;
static GLuint watchedHandles[NUM_WATCHES];
#endif

/*
#ifdef _WIN32
HINSTANCE lib = NULL;
#else
int lib = NULL;
#endif
*/
bool bOpenGlLoaded = FALSE;

ALPHAFUNCproc              glAlphaFunc              = NULL;
ARRAYELEMENTproc           glArrayElement           = NULL;
BEGINproc                  glBegin                  = NULL;
BEGINproc                 _glBegin                  = NULL;
BINDTEXTUREproc            glBindTexture            = NULL;
BINDTEXTUREproc           _glBindTexture            = NULL;
BITMAPproc                 glBitmap                 = NULL;
BLENDFUNCproc              glBlendFunc              = NULL;
CLEARCOLORproc             glClearColor             = NULL;
CLEARDEPTHproc             glClearDepth             = NULL;
CLEARINDEXproc             glClearIndex             = NULL;
CLEARproc                  glClear                  = NULL;
CLIPPLANEproc              glClipPlane              = NULL;
COLOR3Fproc                glColor3f                = NULL;
COLOR3UBproc               glColor3ub               = NULL;
COLOR4Fproc                glColor4f                = NULL;
COLOR4UBproc               glColor4ub               = NULL;
COLORMASKproc              glColorMask              = NULL;
COLORTABLEproc             glColorTable             = NULL;
CULLFACEproc               glCullFace               = NULL;
DELETETEXTURESproc         glDeleteTextures         = NULL;
DELETETEXTURESproc        _glDeleteTextures         = NULL;
DEPTHFUNCproc              glDepthFunc              = NULL;
DEPTHMASKproc              glDepthMask              = NULL;
DEPTHRANGEproc             glDepthRange             = NULL;
DISABLECLIENTSTATEproc     glDisableClientState     = NULL;
DISABLEproc                glDisable                = NULL;
DISABLEproc               _glDisable                = NULL;
DRAWARRAYSproc             glDrawArrays             = NULL;
DRAWBUFFERproc             glDrawBuffer             = NULL;
DRAWELEMENTSproc           glDrawElements           = NULL;
DRAWPIXELSproc             glDrawPixels             = NULL;
ENABLECLIENTSTATEproc      glEnableClientState      = NULL;
ENABLEproc                 glEnable                 = NULL;
ENABLEproc                _glEnable                 = NULL;
ENDproc                    glEnd                    = NULL;
ENDproc                   _glEnd                    = NULL;
EVALCOORD1Fproc            glEvalCoord1f            = NULL;
EVALCOORD2Fproc            glEvalCoord2f            = NULL;
EVALMESH1proc              glEvalMesh1              = NULL;
EVALMESH2proc              glEvalMesh2              = NULL;
EVALPOINT1proc             glEvalPoint1             = NULL;
EVALPOINT2proc             glEvalPoint2             = NULL;
FLUSHproc                  glFlush                  = NULL;
FOGFVproc                  glFogfv                  = NULL;
FOGFproc                   glFogf                   = NULL;
FOGIproc                   glFogi                   = NULL;
FRUSTUMproc                glFrustum                = NULL;
GENTEXTURESproc            glGenTextures            = NULL;
GENTEXTURESproc           _glGenTextures            = NULL;
GETBOOLEANVproc            glGetBooleanv            = NULL;
GETDOUBLEVproc             glGetDoublev             = NULL;
GETERRORproc               glGetError               = NULL;
GETFLOATVproc              glGetFloatv              = NULL;
GETINTEGERVproc            glGetIntegerv            = NULL;
GETSTRINGproc              glGetString              = NULL;
GETTEXIMAGEproc            glGetTexImage            = NULL;
GETTEXLEVELPARAMETERIVproc glGetTexLevelParameteriv = NULL;
HINTproc                   glHint                   = NULL;
INTERLEAVEDARRAYSproc      glInterleavedArrays      = NULL;
ISENABLEDproc              glIsEnabled              = NULL;
LIGHTFVproc                glLightfv                = NULL;
LIGHTMODELFVproc           glLightModelfv           = NULL;
LIGHTMODELFproc            glLightModelf            = NULL;
LIGHTMODELIproc            glLightModeli            = NULL;
LINESTIPPLEproc            glLineStipple            = NULL;
LINEWIDTHproc              glLineWidth              = NULL;
LOADIDENTITYproc           glLoadIdentity           = NULL;
LOADMATRIXFproc            glLoadMatrixf            = NULL;
LOCKARRAYSEXTproc          glLockArraysEXT          = NULL;
MAP1Fproc                  glMap1f                  = NULL;
MAP2Fproc                  glMap2f                  = NULL;
MAPGRID1Fproc              glMapGrid1f              = NULL;
MAPGRID2Dproc              glMapGrid2d              = NULL;
MATERIALFVproc             glMaterialfv             = NULL;
MATRIXMODEproc             glMatrixMode             = NULL;
MULTMATRIXDproc            glMultMatrixd            = NULL;
MULTMATRIXFproc            glMultMatrixf            = NULL;
NORMAL3FVproc              glNormal3fv              = NULL;
NORMAL3Fproc               glNormal3f               = NULL;
ORTHOproc                  glOrtho                  = NULL;
PIXELSTOREIproc            glPixelStorei            = NULL;
PIXELTRANSFERFproc         glPixelTransferf         = NULL;
POINTSIZEproc              glPointSize              = NULL;
POLYGONMODEproc            glPolygonMode            = NULL;
POPATTRIBproc              glPopAttrib              = NULL;
POPMATRIXproc              glPopMatrix              = NULL;
PUSHATTRIBproc             glPushAttrib             = NULL;
PUSHMATRIXproc             glPushMatrix             = NULL;
RASTERPOS2Fproc            glRasterPos2f            = NULL;
RASTERPOS2Iproc            glRasterPos2i            = NULL;
RASTERPOS4Fproc            glRasterPos4f            = NULL;
READBUFFERproc             glReadBuffer             = NULL;
READPIXELSproc             glReadPixels             = NULL;
ROTATEFproc                glRotatef                = NULL;
SCALEFproc                 glScalef                 = NULL;
SCISSORproc                glScissor                = NULL;
SHADEMODELproc             glShadeModel             = NULL;
TEXCOORD2Fproc             glTexCoord2f             = NULL;
TEXENVIproc                glTexEnvi                = NULL;
TEXIMAGE1Dproc             glTexImage1D             = NULL;
TEXIMAGE2Dproc             glTexImage2D             = NULL;
TEXIMAGE2Dproc            _glTexImage2D             = NULL;
TEXPARAMETERIproc          glTexParameteri          = NULL;
TRANSLATEDproc             glTranslated             = NULL;
TRANSLATEFproc             glTranslatef             = NULL;
UNLOCKARRAYSEXTproc        glUnlockArraysEXT        = NULL;
VERTEX2Fproc               glVertex2f               = NULL;
VERTEX2Iproc               glVertex2i               = NULL;
VERTEX3FVproc              glVertex3fv              = NULL;
VERTEX3FVproc             _glVertex3fv              = NULL;
VERTEX3Fproc               glVertex3f               = NULL;
VERTEX4FVproc              glVertex4fv              = NULL;
VERTEXPOINTERproc          glVertexPointer          = NULL;
VIEWPORTproc               glViewport               = NULL;

WGETPROCADDRESSproc        rwglGetProcAddress;
WSWAPBUFFERSproc           rwglSwapBuffers;
#ifdef _WIN32
WCREATECONTEXTproc         rwglCreateContext;
WDELETECONTEXTproc         rwglDeleteContext;
WMAKECURRENTproc           rwglMakeCurrent;
WCHOOSEPIXELFORMATproc     rwglChoosePixelFormat;
WSETPIXELFORMATproc        rwglSetPixelFormat;
WGETPIXELFORMATproc        rwglGetPixelFormat;
WDESCRIBEPIXELFORMATproc   rwglDescribePixelFormat;
#endif

#if VERTEX_WRAPPERS
FILE* vout;
void APIENTRY myVertex3fv(GLfloat const* v)
{
    static GLfloat last[3];
    static GLfloat current[3];
    if (v[0] > 735916800.0f ||
        v[0] < -735916800.0f)
    {
        __asm int 3 ;
        fprintf(vout, "* %10.5f %10.5f %10.5f\n", v[0], v[1], v[2]);
        current[0] = last[0];
        current[1] = last[1];
        current[2] = last[2];
    }
    else
    {
        fprintf(vout, "  %10.5f %10.5f %10.5f\n", v[0], v[1], v[2]);
        last[0] = current[0] = v[0];
        last[1] = current[1] = v[1];
        last[2] = current[2] = v[2];
    }
    _glVertex3fv(current);
}
#endif

#if WRAPPERS
void APIENTRY myBindTexture(GLenum target, GLuint textureName)
{
    if (textureName == lastBound)
    {
        return;
    }
    _glBindTexture(target, textureName);
    lastBound = textureName;
}
#endif

#if GENTEX_WRAPPERS
static FILE* genOut = NULL;
#if 1
void APIENTRY myGenTextures(GLsizei n, GLuint* textureNames)
{
    _glGenTextures(n, textureNames);
    glFlush();
}
#else
void APIENTRY myGenTextures(GLsizei n, GLuint* textureNames)
{
    char *genOutFileName;
    int i;

    _glGenTextures(n, textureNames);
    if (genOut == NULL)
    {
        genOutFileName = filePathPrepend("gentex.txt", FF_UserSettingsPath);

        if (!fileMakeDestinationDirectory(genOutFileName))
            return;

        genOut = fopen(genOutFileName, "wt");
        if (genOut == NULL)
        {
            return;
        }
    }
    fprintf(genOut, "gen %d:", n);
    for (i = 0; i < n; i++)
    {
        fprintf(genOut, " %u", textureNames[i]);
    }
    fprintf(genOut, "\n");
}
#endif //1
#endif //GENTEX_WRAPPERS

#if DELTEX_WRAPPERS
void APIENTRY myDeleteTextures(GLsizei n, GLuint const* textures)
{
    _glDeleteTextures(n, textures);
    glFlush();
}
#endif

#if BEGIN_WRAPPERS
static GLenum currentPrimitive = GL_NONE;
void APIENTRY myBegin(GLenum primitive)
{
    if (primitive == GL_LINES ||
        primitive == GL_LINE_STRIP ||
        primitive == GL_LINE_LOOP)
    {
        currentPrimitive = GL_POINTS;
        __asm int 3 ;
    }
    else
    {
        currentPrimitive = primitive;
    }
    _glBegin(currentPrimitive);
}
#endif

static int int3 = -1;

void glDLLToggleWatch(unsigned int on)
{
    int3 = (int)on;
}

#if TEXIMAGE_WRAPPERS
void APIENTRY myTexImage2D(GLenum target, GLint level, GLint internalFormat,
                            GLsizei width, GLsizei height, GLint border,
                            GLenum format, GLenum type,
                            GLvoid const* pixels)
{
#if WRAPPERS
    int index;

    for (index = 0; index < NUM_WATCHES; index++)
    {
        if (lastBound == watchedHandles[index])
        {
            //we're modifying a texture we're watching
            if (int3)
            {
                __asm int 3 ;
            }
        }
    }
#endif
    _glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
}
#endif

void APIENTRY noDitheredAlphaEnable(GLenum cap)
{
    if (cap == GL_BLEND)
    {
        _glDisable(GL_DITHER);
        _glEnable(GL_BLEND);
    }
    else
    {
        _glEnable(cap);
    }
}

void APIENTRY noDitheredAlphaDisable(GLenum cap)
{
    if (cap == GL_BLEND)
    {
        _glEnable(GL_DITHER);
        _glDisable(GL_BLEND);
    }
    else
    {
        _glDisable(cap);
    }
}

void glDLLReset(void)
{
#if TEXIMAGE_WRAPPERS
    int index;

    watchedIndex = 0;
    for (index = 0; index < NUM_WATCHES; index++)
    {
        watchedHandles[index] = 0;
    }
#endif

#if WRAPPERS
    myBindTexture(GL_TEXTURE_2D, 0);
#endif

#if GENTEX_WRAPPERS
    if (genOut != NULL)
    {
        fclose(genOut);
        genOut = NULL;
    }
#endif
}

void glDLLGetGLCompat(void)
{
    int DynalinkFailed = 0;

    if (!bOpenGlLoaded)
    {
        return;
    }

    DYNALINK_GL_FUNCTION(DrawPixels);
    DYNALINK_GL_FUNCTION(RasterPos2f);
    DYNALINK_GL_FUNCTION(Clear);
    DYNALINK_GL_FUNCTION(Color3ub);
    DYNALINK_DGL_FUNCTION(Enable);
    DYNALINK_DGL_FUNCTION(Disable);
    if (bitTest(gDevcaps2, DEVSTAT2_NO_DALPHA))
    {
        glEnable  = noDitheredAlphaEnable;
        glDisable = noDitheredAlphaDisable;
    }
    else
    {
        glEnable  = _glEnable;
        glDisable = _glDisable;
    }

    dbgAssertOrIgnore(!DynalinkFailed);
}

void glDLLWatch(GLuint handle)
{
#if TEXIMAGE_WRAPPERS
    watchedHandles[watchedIndex++] = handle;
#endif
}

extern bool mainAllow3DNow;

#define glDLLEnvironment() \
    if (!mainAllow3DNow)\
    {\
        putenv("__GL_FORCE_K3D=0");\
    }\
    if (!mainAllowKatmai)\
    {\
        putenv("__GL_FORCE_KNI=0");\
    }

/* TC 2003-10-01:
 * I've made attempts to provide dummy functions in rgl to make SDL believe it
 * is a valid OpenGL implementation.  Only SDL_GL_LoadLibrary() and
 * SDL_GL_GetProcAddress() are expected to work properly; SDL_GL_SwapBuffers()
 * should only be called if you know the DLL is actually an OpenGL DLL and not
 * an rgl DLL (check the value of the "RGL" global variable).
 *
 * This currently only works with WGL (Windows) and glX.
 */
GLboolean glDLLGetProcs(char* dllName)
{
    int DynalinkFailed = 0;
    Uint32 sdl_flags;
#if VERTEX_WRAPPERS
    char *voutFileName;
#endif

#if TEXIMAGE_WRAPPERS
    int index;

    watchedIndex = 0;
    for (index = 0; index < NUM_WATCHES; index++)
    {
        watchedHandles[index] = 0;
    }
#endif

    /* Make sure SDL video is initialized. */
    sdl_flags = SDL_WasInit(SDL_INIT_EVERYTHING);
    if (!sdl_flags)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
            return GL_FALSE;
    }
    else if (!(sdl_flags & SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
            return GL_FALSE;
    }

    glDLLEnvironment();

#ifdef _MACOSX_FIX_GL
    dllName = NULL;
#endif

    //try loading the .DLL
    if (SDL_GL_LoadLibrary(dllName) == -1)
    {
        fprintf(stderr, "SDL_GL_LoadLibrary(%s): %s\n",
            (dllName ? dllName : "NULL"), SDL_GetError());
        return GL_FALSE;
    }

    bOpenGlLoaded = TRUE;

#if VERTEX_WRAPPERS
    voutFileName = filePathPrepend("vout.dat", FF_UserSettingsPath);

    if (fileMakeDestinationDirectory(voutFileName))
        vout = fopen(voutFileName, "wt");
#endif

    DYNALINK_GL_FUNCTION(AlphaFunc);
    DYNALINK_DGL_FUNCTION(Begin);
#if BEGIN_WRAPPERS
    glBegin = myBegin;
#else
    glBegin = _glBegin;
#endif
    DYNALINK_DGL_FUNCTION(BindTexture);
#if WRAPPERS
    glBindTexture = myBindTexture;
#else
    glBindTexture = _glBindTexture;
#endif
    DYNALINK_GL_FUNCTION(Bitmap);
    DYNALINK_GL_FUNCTION(BlendFunc);
    DYNALINK_GL_FUNCTION(Clear);
    DYNALINK_GL_FUNCTION(ClearColor);
    DYNALINK_GL_FUNCTION(ClearDepth);
    DYNALINK_GL_FUNCTION(ClearIndex);
    DYNALINK_GL_FUNCTION(Color3f);
    DYNALINK_GL_FUNCTION(Color3ub);
    DYNALINK_GL_FUNCTION(Color4f);
    DYNALINK_GL_FUNCTION(Color4ub);
    DYNALINK_GL_FUNCTION(ColorMask);

    glColorTable = (COLORTABLEproc)SDL_GL_GetProcAddress("glColorTableEXT");

    DYNALINK_GL_FUNCTION(CullFace);
    DYNALINK_DGL_FUNCTION(DeleteTextures);
#if DELTEX_WRAPPERS
    glDeleteTextures = myDeleteTextures;
#else
    glDeleteTextures = _glDeleteTextures;
#endif
    DYNALINK_GL_FUNCTION(DepthFunc);
    DYNALINK_GL_FUNCTION(DepthMask);
    DYNALINK_GL_FUNCTION(DepthRange);
    DYNALINK_DGL_FUNCTION(Disable);
    DYNALINK_DGL_FUNCTION(Enable);
    if (bitTest(gDevcaps2, DEVSTAT2_NO_DALPHA))
    {
        glDisable = noDitheredAlphaDisable;
        glEnable  = noDitheredAlphaEnable;
    }
    else
    {
        glDisable = _glDisable;
        glEnable  = _glEnable;
    }
    DYNALINK_GL_FUNCTION(DrawArrays);
    DYNALINK_GL_FUNCTION(DrawElements);
    DYNALINK_GL_FUNCTION(DrawBuffer);
    DYNALINK_GL_FUNCTION(DrawPixels);
    DYNALINK_GL_FUNCTION(End);
    DYNALINK_GL_FUNCTION(EvalCoord1f);
    DYNALINK_GL_FUNCTION(EvalCoord2f);
    DYNALINK_GL_FUNCTION(EvalMesh1);
    DYNALINK_GL_FUNCTION(EvalMesh2);
    DYNALINK_GL_FUNCTION(EvalPoint1);
    DYNALINK_GL_FUNCTION(EvalPoint2);
    DYNALINK_GL_FUNCTION(Flush);
    DYNALINK_GL_FUNCTION(Fogf);
    DYNALINK_GL_FUNCTION(Fogfv);
    DYNALINK_GL_FUNCTION(Fogi);
    DYNALINK_GL_FUNCTION(Frustum);
    DYNALINK_DGL_FUNCTION(GenTextures);
#if GENTEX_WRAPPERS
    glGenTextures = myGenTextures;
#else
    glGenTextures = _glGenTextures;
#endif
    DYNALINK_GL_FUNCTION(GetDoublev);
    DYNALINK_GL_FUNCTION(GetError);
    DYNALINK_GL_FUNCTION(GetFloatv);
    DYNALINK_GL_FUNCTION(GetIntegerv);
    DYNALINK_GL_FUNCTION(GetBooleanv);
    DYNALINK_GL_FUNCTION(GetString);
    DYNALINK_GL_FUNCTION(GetTexLevelParameteriv);
    DYNALINK_GL_FUNCTION(Hint);
    DYNALINK_GL_FUNCTION(InterleavedArrays);
    DYNALINK_GL_FUNCTION(IsEnabled);
    DYNALINK_GL_FUNCTION(LightModelf);
    DYNALINK_GL_FUNCTION(LightModelfv);
    DYNALINK_GL_FUNCTION(LightModeli);
    DYNALINK_GL_FUNCTION(Lightfv);
    DYNALINK_GL_FUNCTION(LineStipple);
    DYNALINK_GL_FUNCTION(LineWidth);
    DYNALINK_GL_FUNCTION(LoadIdentity);
    DYNALINK_GL_FUNCTION(LoadMatrixf);
    DYNALINK_GL_FUNCTION(Map1f);
    DYNALINK_GL_FUNCTION(Map2f);
    DYNALINK_GL_FUNCTION(MapGrid1f);
    DYNALINK_GL_FUNCTION(MapGrid2d);
    DYNALINK_GL_FUNCTION(Materialfv);
    DYNALINK_GL_FUNCTION(MatrixMode);
    DYNALINK_GL_FUNCTION(MultMatrixd);
    DYNALINK_GL_FUNCTION(MultMatrixf);
    DYNALINK_GL_FUNCTION(Normal3f);
    DYNALINK_GL_FUNCTION(Normal3fv);
    DYNALINK_GL_FUNCTION(Ortho);
    DYNALINK_GL_FUNCTION(PixelStorei);
    DYNALINK_GL_FUNCTION(PixelTransferf);
    DYNALINK_GL_FUNCTION(PointSize);
    DYNALINK_GL_FUNCTION(PolygonMode);
    DYNALINK_GL_FUNCTION(PopAttrib);
    DYNALINK_GL_FUNCTION(PopMatrix);
    DYNALINK_GL_FUNCTION(PushAttrib);
    DYNALINK_GL_FUNCTION(PushMatrix);
    DYNALINK_GL_FUNCTION(RasterPos2f);
    DYNALINK_GL_FUNCTION(RasterPos2i);
    DYNALINK_GL_FUNCTION(RasterPos4f);
    DYNALINK_GL_FUNCTION(ReadBuffer);
    DYNALINK_GL_FUNCTION(ReadPixels);
    DYNALINK_GL_FUNCTION(Rotatef);
    DYNALINK_GL_FUNCTION(Scalef);
    DYNALINK_GL_FUNCTION(Scissor);
    DYNALINK_GL_FUNCTION(ShadeModel);
    DYNALINK_GL_FUNCTION(TexCoord2f);
    DYNALINK_GL_FUNCTION(TexEnvi);
    DYNALINK_GL_FUNCTION(TexImage1D);
#if TEXIMAGE_WRAPPERS
    glTexImage2D = myTexImage2D;
    DYNALINK_DGL_FUNCTION(TexImage2D);
#else
    DYNALINK_GL_FUNCTION(TexImage2D);
#endif
    DYNALINK_GL_FUNCTION(TexParameteri);
    DYNALINK_GL_FUNCTION(Translated);
    DYNALINK_GL_FUNCTION(Translatef);
    DYNALINK_GL_FUNCTION(Vertex2f);
    DYNALINK_GL_FUNCTION(Vertex2i);
    DYNALINK_GL_FUNCTION(Vertex3f);
#if VERTEX_WRAPPERS
    glVertex3fv = myVertex3fv;
    DYNALINK_DGL_FUNCTION(Vertex3fv);
#else
    DYNALINK_GL_FUNCTION(Vertex3fv);
#endif
    DYNALINK_GL_FUNCTION(Vertex4fv);
    DYNALINK_GL_FUNCTION(Viewport);
    DYNALINK_GL_FUNCTION(ClipPlane);
    DYNALINK_GL_FUNCTION(GetTexImage);
    DYNALINK_GL_FUNCTION(VertexPointer);
    DYNALINK_GL_FUNCTION(EnableClientState);
    DYNALINK_GL_FUNCTION(DisableClientState);
    DYNALINK_GL_FUNCTION(ArrayElement);

    if (DynalinkFailed)
    {
        return GL_FALSE;
    }

    rwglGetProcAddress = (WGETPROCADDRESSproc)SDL_GL_GetProcAddress("rglGetProcAddress");
    if (!rwglGetProcAddress)
    {
        /* Not an RGL lib, must be regular OpenGL. */
        rwglGetProcAddress = (WGETPROCADDRESSproc)SDL_GL_GetProcAddress;
    }

    DYNALINK_GL_FUNCTION(LockArraysEXT);
    DYNALINK_GL_FUNCTION(UnlockArraysEXT);

    return GL_TRUE;
}
