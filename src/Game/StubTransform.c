#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>

typedef struct tagvertex
{
    float x;                               // X component of this vertex.
    float y;                               // Y component of this vertex.
    float z;                               // Z component of this vertex.
    int   iVertexNormal;                   // Index into the point normal list.
} vertexentry;

typedef struct
{
    float x, y, z, w;
} hvector;

typedef struct
{
    float m11,m21,m31,m41, m12,m22,m32,m42, m13,m23,m33,m43, m14,m24,m34,m44;
} hmatrix;

/*-----------------------------------------------------------------------------
    Name        : transDetermineKatmaiSupport
    Description : determines whether Katmai New Instructions are supported by the
                  processor & the OS.  kind of guesses under NT
    Inputs      : haveKatmai - !0 if CPU.XMM bit set
    Outputs     :
    Return      : 1 or 0 (does or doesn't)
----------------------------------------------------------------------------*/
int transDetermineKatmaiSupport(unsigned int haveKatmai)
{
  return 0;
}

/*-----------------------------------------------------------------------------
    Name        : transCanSupportKatmai
    Description : wrapper for global variable set in transDetermineKatmaiSupport()
    Inputs      :
    Outputs     :
    Return      : 1 or 0 (can or can't)
----------------------------------------------------------------------------*/
int transCanSupportKatmai(void)
{
    return 0;
}

/*-----------------------------------------------------------------------------
    Name        : transTransformCompletely_intrin
    Description : transform a bunch of vertices by a 3D matrix, then by a perspective matrix
    Inputs      : nVerts - number of vertices (not vectored)
                  dest - destination vertices
                  source - source vertices
                  m0 - 3D matrix, 4x4
                  m1 - perspective matrix, 4x4
    Outputs     : dest is filled
    Return      :
----------------------------------------------------------------------------*/
void transTransformCompletely_intrin(
    int nVerts, hvector* dest, vertexentry* source, float* m0, float* m1)
{
}

void transTransformCompletely_xmm(
    int nVerts, hvector* dest, vertexentry* source, float* M0, float* M1)
{
}

/*-----------------------------------------------------------------------------
    Name        : transTransformVertexList_intrin
    Description : transform a bunch of vertices by a 3D matrix
    Inputs      : nVerts - number of vertices (not vectored)
                  dest - destination vertices, not used (kept in temporary vectored storage instead)
                  source - source vertices
                  m - 3D matrix, 4x4
    Outputs     : sOutputVerts is filled
    Return      :
----------------------------------------------------------------------------*/
void transTransformVertexList_intrin(int nVerts, hvector* dest, vertexentry* source, float* m)
{
}

/*-----------------------------------------------------------------------------
    Name        : transPerspectiveTransform_intrin
    Description : transform a bunch of vertices by a 1-point projection matrix.
                  this fn continues transTransformVertexList_intrin(..), and hence
                  uses its temporary storage for setup
    Inputs      : nVerts - number of vertices (not vectored)
                  dest - destination vertices
                  source - source vertices, not used
                  m - projection matrix
    Outputs     : dest is filled
    Return      :
----------------------------------------------------------------------------*/
void transPerspectiveTransform_intrin(int nVerts, hvector* dest, hvector* source, float* m)
{
}

/*-----------------------------------------------------------------------------
    Name        : transGeneralPerspectiveTransform_intrin
    Description : transform a bunch of vertices by a general projection matrix.
                  this fn continues transTransformVertexList_intrin(..), and hence
                  uses its temporary storage for setup
    Inputs      : nVerts - number of vertices (not vectored)
                  dest - destination vertices
                  source - source vertices, not used
                  m - general projection matrix
    Outputs     : dest is filled
    Return      :
----------------------------------------------------------------------------*/
void transGeneralPerspectiveTransform_intrin(int nVerts, hvector* dest, hvector* source, float* m)
{
}

/*-----------------------------------------------------------------------------
    Name        : kniTransFreeVertexLists
    Description : C-callable, frees vertex list contents
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void kniTransFreeVertexLists(void)
{
}

/*-----------------------------------------------------------------------------
    Name        : transSetKatmaiSupport
    Description : forces / disallows Katmai support at the user's whim
    Inputs      : on - 1 or 0 (enable or disable)
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void transSetKatmaiSupport(unsigned int on)
{
}
