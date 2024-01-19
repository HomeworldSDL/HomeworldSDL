/*=============================================================================
    Name    : transformer.c
    Purpose : provides vertex buffer transformation

    Created 10/8/1998 by
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "Transformer.h"
#include "Memory.h"
#include "main.h"

#define FPTR dword ptr
#define FSIZE 4
#define FSIZE_STR "4"


/*=============================================================================
    Data
=============================================================================*/

#if defined (_MSC_VER)
#define S(x) [esi + FSIZE*x]
#define D(x) [edi + FSIZE*x]
#define M(x) [edx + FSIZE*x]
#elif defined (__GNUC__) && defined (__i386__)
#define S(x) FSIZE_STR "*" #x "(%%esi)"
#define D(x) FSIZE_STR "*" #x "(%%edi)"
#define M(x) FSIZE_STR "*" #x "(%%edx)"
#endif

static sdword nVerts;
hvector* eyeVertexList = NULL;
hvector* clipVertexList = NULL;


//bits of the CPUID register
#define FXSR_BIT 0x1000000
#define XMM_BIT  0x2000000

//knitransform.cpp prototypes
int transDetermineKatmaiSupport(unsigned int);
int transCanSupportKatmai(void);
void transTransformCompletely_intrin(sdword, hvector*, vertexentry*, hmatrix*, hmatrix*);
void transTransformCompletely_xmm(sdword, hvector*, vertexentry*, hmatrix*, hmatrix*);
void transTransformVertexList_intrin(sdword, hvector*, vertexentry*, hmatrix*);
void transPerspectiveTransform_intrin(sdword, hvector*, hvector*, hmatrix*);
void transGeneralPerspectiveTransform_intrin(sdword, hvector*, hvector*, hmatrix*);
void kniTransFreeVertexLists(void);
void transSetKatmaiSupport(unsigned int);

static udword haveKatmai, haveFXSR;
static int useKatmai;

typedef void (*transVertexList_proc)(sdword, hvector*, vertexentry*, hmatrix*);
typedef void (*transPerspective_proc)(sdword, hvector*, hvector*, hmatrix*);
static transVertexList_proc transVertexList;
static transPerspective_proc transPerspective;
static transPerspective_proc transGeneral;

/*=============================================================================
    Code
=============================================================================*/

/*-----------------------------------------------------------------------------
    Name        : transStartup
    Description : starts up the transformer module and determines if SSE (KNI) code should be utilized.
    See also    : transShutdown()
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void transStartup(void)
{
#if defined (__GNUC__) && defined (__i386__)
    static unsigned int cpu_edx;
#endif

    haveKatmai = 0;

    transShutdown();

    if (mainForceKatmai)
    {
#ifndef _MACOSX_FIX_MISC
        transSetKatmaiSupport(1);
#endif
        haveKatmai = 1;
        haveFXSR = 1;
    }
    else if (!mainAllowKatmai)
    {
#ifndef _MACOSX_FIX_MISC
        transSetKatmaiSupport(0);
#endif
        haveKatmai = 0;
        haveFXSR = 0;
        
#ifndef _MACOSX_FIX_MISC
        (void)transDetermineKatmaiSupport(haveKatmai);
#endif
    }
}

/*-----------------------------------------------------------------------------
    Name        : transShutdown
    Description : shuts down the transformer module (deallocates memory)
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void transShutdown(void)
{
    nVerts = 0;
    if (eyeVertexList != NULL)
    {
        memFree(eyeVertexList);
        eyeVertexList = NULL;
    }
    if (clipVertexList != NULL)
    {
        memFree(clipVertexList);
        clipVertexList = NULL;
    }

#ifndef _MACOSX_FIX_MISC
    kniTransFreeVertexLists();
#endif
}

/*-----------------------------------------------------------------------------
    Name        : transFree
    Description : wrapper for calling from C++
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void transFree(void* ptr)
{
    memFree(ptr);
}

/*-----------------------------------------------------------------------------
    Name        : transAlloc
    Description : wrapper for calling from C++
    Inputs      :
    Outputs     :
    Return      :
    ASSERT      : memAlloc returns 16 byte aligned data, or multiple of
----------------------------------------------------------------------------*/
void* transAlloc(int n)
{
    return (void*)memAlloc(n, "Katmai mem", NonVolatile);
}

/*-----------------------------------------------------------------------------
    Name        : transGrowVertexLists
    Description : grows the vertex lists to accomodate an increasing number of vertices.
                  performance is expected to be very poor for a frame, then stable
                  afterwards.  TODO: transGrowAndShrinkVertexLists, a more memory-
                  conscious version of this pathetic fn
    Inputs      : nVertices - number of vertices
    Outputs     : preVertexList, xVertexList are re/allocated
    Return      :
----------------------------------------------------------------------------*/
void transGrowVertexLists(sdword nVertices)
{
    if (eyeVertexList == NULL)
    {
        nVerts = nVertices;
        eyeVertexList = (hvector*)memAlloc(nVerts * sizeof(hvector), "eye vertex list", NonVolatile);
        clipVertexList = (hvector*)memAlloc(nVerts * sizeof(hvector), "clip vertex list", NonVolatile);
    }

    if (nVerts < nVertices)
    {
        nVerts = nVertices;
        eyeVertexList = (hvector*)memRealloc(
            eyeVertexList, nVerts * sizeof(hvector), "eye vertex list", NonVolatile);
        clipVertexList = (hvector*)memRealloc(
            clipVertexList, nVerts * sizeof(hvector), "clip vertex list", NonVolatile);
    }
}

/*-----------------------------------------------------------------------------
    Name        : transTransformVertexList
    Description : transforms vertices in mesh format (vertexentry) by a 3D
                  (rotation + translation) matrix
    Inputs      : n - number of vertices
                  dest - transformed output vertices
                  source - input vertices
                  m - the matrix
    Outputs     : dest is filled
    Return      :
    Note        : need PII version of this, fxch stalls the pipe
----------------------------------------------------------------------------*/
/*
 * vEye[0] = m[0] * vObj[0] + m[4] * vObj[1] +  m[8] * vObj[2] + m[12]
 * vEye[1] = m[1] * vObj[0] + m[5] * vObj[1] +  m[9] * vObj[2] + m[13]
 * vEye[2] = m[2] * vObj[0] + m[6] * vObj[1] + m[10] * vObj[2] + m[14]
 * vEye[3] = 1.0f
 *
 * assumes next stage in pipe ignores vEye[3]
 */
void transTransformVertexList(sdword n, hvector* dest, vertexentry* source, hmatrix* m)
{
    transVertexList(n, dest, source, m);
}

/*-----------------------------------------------------------------------------
    Name        : transSinglePerspectiveTransform
    Description : transforms a vertex by a perspective matrix
    Inputs      : screenSpace - output screenspace vector
                  projection - projection matrix
                  cameraSpace - input cameraspace vector, ie. worldspace after modelview transform
    Outputs     : screenSpace hvector is computed
    Return      :
    Note        : *ONLY* use a perspective matrix here
----------------------------------------------------------------------------*/
void transSinglePerspectiveTransform(hvector* screenSpace, hmatrix* projection, hvector* cameraSpace)
{
    real32* m = (real32*)projection;
    real32* out = (real32*)screenSpace;
    real32* in  = (real32*)cameraSpace;

    out[0] = m[0]  * in[0] +  m[8] * in[2];
    out[1] = m[5]  * in[1] +  m[9] * in[2];
    out[2] = m[10] * in[2] + m[14] * in[3];
    out[3] = -in[2];
}

/*-----------------------------------------------------------------------------
    Name        : transSingleTotalTransform
    Description : transforms a vertex by a modelview & projection matrix
    Inputs      : screenSpace - output screenspace vector
                  modelview, projection - matrices
                  worldSpace - input worldspace vector
    Outputs     : screenSpace vector is computed
    Return      :
    Note        : *ONLY* use a perspective matrix for projection
----------------------------------------------------------------------------*/
void transSingleTotalTransform(vector* screenSpace, hmatrix* modelview, hmatrix* projection, vector* worldSpace)
{
}

/*-----------------------------------------------------------------------------
    Name        : transPerspectiveTransform
    Description : transforms vertices by a perspective matrix
    Inputs      : n - number of vertices
                  dest - transformed output vertices
                  source - input vertices
                  m - the matrix
    Outputs     : dest is filled
    Return      :
    Note        : *ONLY* use a perspective matrix here
----------------------------------------------------------------------------*/
/*
 * vClip[0] = m[0]  * vEye[0] +  m[8] * vEye[2]
 * vClip[1] = m[5]  * vEye[1] +  m[9] * vEye[2]
 * vClip[2] = m[10] * vEye[2] + m[14] * vEye[3]
 * vClip[3] = -vEye[2]
 *
 * assumes vEye[3] == 1.0f
 */
void transPerspectiveTransform(sdword n, hvector* dest, hvector* source, hmatrix* m)
{
    transPerspective(n, dest, source, m);
}

/*-----------------------------------------------------------------------------
    Name        : transTransformCompletely
    Description : transform 1st by a 3D matrix, then by a perspective matrix
    Inputs      : n - number of vertices
                  dest - transformed output vertices
                  intermed - temp storage for output from 3D transform
                  source - input vertices
                  m0 - 3D matrix
                  m1 - perspective matrix
    Outputs     : dest is filled
    Return      :
----------------------------------------------------------------------------*/
void transTransformCompletely(
    sdword n, hvector* dest, hvector* intermed, vertexentry* source, hmatrix* m0, hmatrix* m1)
{
}

/*-----------------------------------------------------------------------------
    Name        : transGeneralPerspectiveTransform
    Description : transforms 3D vertices by a general matrix
    Inputs      : n - number of vertices
                  dest - output vertices
                  source - input vertices
                  m - matrix
    Outputs     : dest is filled
    Return      :
----------------------------------------------------------------------------*/
/*
    sdword i;
    real32  ex, ey, ez;
    real32* mat = (real32*)m;

    for (i = 0; i < n; i++)
    {
        ex = source[i].x;
        ey = source[i].y;
        ez = source[i].z;
        dest[i].x = mat[0] * ex + mat[4] * ey + mat[8]  * ez + mat[12];
        dest[i].y = mat[1] * ex + mat[5] * ey + mat[9]  * ez + mat[13];
        dest[i].z = mat[2] * ex + mat[6] * ey + mat[10] * ez + mat[14];
        dest[i].w = mat[3] * ex + mat[7] * ey + mat[11] * ez + mat[15];
    }
*/
void transGeneralPerspectiveTransform(sdword n, hvector* dest, hvector* source, hmatrix* m)
{
    transGeneral(n, dest, source, m);
}

/*-----------------------------------------------------------------------------
    Name        : transPerspectiveMatrix
    Description : determines whether a given matrix may use the faster perspective
                  transform function
    Inputs      : m - the matrix to test
    Outputs     :
    Return      : TRUE if transPerspectiveTransform will suffice, FALSE if the matrix
                  is too general
----------------------------------------------------------------------------*/
bool transPerspectiveMatrix(hmatrix* m)
{
    real32* mat = (real32*)m;

    if (mat[4] == 0.0f && mat[12] == 0.0f &&
        mat[1] == 0.0f && mat[13] == 0.0f &&
        mat[2] == 0.0f && mat[6] == 0.0f &&
        mat[3] == 0.0f && mat[7] == 0.0f &&
        mat[11] == -1.0f && mat[15] == 0.0f)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

