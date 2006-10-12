// =============================================================================
//  Matrix.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created June 1997 by Gary Shaw
// =============================================================================

#ifndef ___MATRIX_H
#define ___MATRIX_H

#include "Types.h"

// forward declarations
struct hvector;
struct vector;

// INTERFACE -------------------------------------------------------------------

typedef struct hmatrix
{
/*-----------------------------------------------------------------------------
    Conceptually, and mathematically, you should think of the matrix as follows:

    |m11 m12 m13 m14|
    |m21 m22 m23 m24|
    |m31 m32 m33 m34|
    |m41 m42 m43 m44|

    In memory, it is stored as follows to be compatible with GameGL 
-----------------------------------------------------------------------------*/
    real32 m11,m21,m31,m41, m12,m22,m32,m42, m13,m23,m33,m43, m14,m24,m34,m44;

} hmatrix;


typedef struct matrix
{
/*-----------------------------------------------------------------------------
    Conceptually, and mathematically, you should think of the matrix as follows:

    |m11 m12 m13|
    |m21 m22 m23|
    |m31 m32 m33|

    In memory, it is stored as follows to be compatible with GameGL 
-----------------------------------------------------------------------------*/
    real32 m11,m21,m31, m12,m22,m32, m13,m23,m33;

} matrix;


// -----------------------------------------------------------------------------
//  Homogenous matrix macros:

#define hmatGetHVectFromHMatrixCol1(hvec,hmat)   \
    (hvec).x = (hmat).m11;                       \
    (hvec).y = (hmat).m21;                       \
    (hvec).z = (hmat).m31;                       \
    (hvec).w = (hmat).m41

#define hmatGetHVectFromHMatrixCol2(hvec,hmat)   \
    (hvec).x = (hmat).m12;                       \
    (hvec).y = (hmat).m22;                       \
    (hvec).z = (hmat).m32;                       \
    (hvec).w = (hmat).m42

#define hmatGetHVectFromHMatrixCol3(hvec,hmat)   \
    (hvec).x = (hmat).m13;                       \
    (hvec).y = (hmat).m23;                       \
    (hvec).z = (hmat).m33;                       \
    (hvec).w = (hmat).m43

#define hmatGetHVectFromHMatrixCol4(hvec,hmat)   \
    (hvec).x = (hmat).m14;                       \
    (hvec).y = (hmat).m24;                       \
    (hvec).z = (hmat).m34;                       \
    (hvec).w = (hmat).m44

// -----------------------------------------------------------------------------

#define hmatPutHVectIntoHMatrixCol1(hvec,hmat)   \
    (hmat).m11 = (hvec).x;                       \
    (hmat).m21 = (hvec).y;                       \
    (hmat).m31 = (hvec).z;                       \
    (hmat).m41 = (hvec).w

#define hmatPutHVectIntoHMatrixCol2(hvec,hmat)   \
    (hmat).m12 = (hvec).x;                       \
    (hmat).m22 = (hvec).y;                       \
    (hmat).m32 = (hvec).z;                       \
    (hmat).m42 = (hvec).w

#define hmatPutHVectIntoHMatrixCol3(hvec,hmat)   \
    (hmat).m13 = (hvec).x;                       \
    (hmat).m23 = (hvec).y;                       \
    (hmat).m33 = (hvec).z;                       \
    (hmat).m43 = (hvec).w

#define hmatPutHVectIntoHMatrixCol4(hvec,hmat)   \
    (hmat).m14 = (hvec).x;                       \
    (hmat).m24 = (hvec).y;                       \
    (hmat).m34 = (hvec).z;                       \
    (hmat).m44 = (hvec).w

// -----------------------------------------------------------------------------

#define hmatGetVectFromHMatrixCol1(vec,hmat)     \
    (vec).x = (hmat).m11;                        \
    (vec).y = (hmat).m21;                        \
    (vec).z = (hmat).m31

#define hmatGetVectFromHMatrixCol2(vec,hmat)     \
    (vec).x = (hmat).m12;                        \
    (vec).y = (hmat).m22;                        \
    (vec).z = (hmat).m32

#define hmatGetVectFromHMatrixCol3(vec,hmat)     \
    (vec).x = (hmat).m13;                        \
    (vec).y = (hmat).m23;                        \
    (vec).z = (hmat).m33

#define hmatGetVectFromHMatrixCol4(vec,hmat)     \
    (vec).x = (hmat).m14;                        \
    (vec).y = (hmat).m24;                        \
    (vec).z = (hmat).m34

// -----------------------------------------------------------------------------

#define hmatPutVectIntoHMatrixCol1(vec,hmat)     \
    (hmat).m11 = (vec).x;                        \
    (hmat).m21 = (vec).y;                        \
    (hmat).m31 = (vec).z;

#define hmatPutVectIntoHMatrixCol2(vec,hmat)     \
    (hmat).m12 = (vec).x;                        \
    (hmat).m22 = (vec).y;                        \
    (hmat).m32 = (vec).z;

#define hmatPutVectIntoHMatrixCol3(vec,hmat)     \
    (hmat).m13 = (vec).x;                        \
    (hmat).m23 = (vec).y;                        \
    (hmat).m33 = (vec).z;

#define hmatPutVectIntoHMatrixCol4(vec,hmat)     \
    (hmat).m14 = (vec).x;                        \
    (hmat).m24 = (vec).y;                        \
    (hmat).m34 = (vec).z;

// -----------------------------------------------------------------------------
//  3 X 3 matrix macros:

#define matGetVectFromMatrixCol1(vec,mat)        \
    (vec).x = (mat).m11;                         \
    (vec).y = (mat).m21;                         \
    (vec).z = (mat).m31;

#define matGetVectFromMatrixCol2(vec,mat)        \
    (vec).x = (mat).m12;                         \
    (vec).y = (mat).m22;                         \
    (vec).z = (mat).m32;

#define matGetVectFromMatrixCol3(vec,mat)        \
    (vec).x = (mat).m13;                         \
    (vec).y = (mat).m23;                         \
    (vec).z = (mat).m33;

// -----------------------------------------------------------------------------

#define matPutVectIntoMatrixCol1(vec,mat)        \
    (mat).m11 = (vec).x;                         \
    (mat).m21 = (vec).y;                         \
    (mat).m31 = (vec).z;

#define matPutVectIntoMatrixCol2(vec,mat)        \
    (mat).m12 = (vec).x;                         \
    (mat).m22 = (vec).y;                         \
    (mat).m32 = (vec).z;

#define matPutVectIntoMatrixCol3(vec,mat)        \
    (mat).m13 = (vec).x;                         \
    (mat).m23 = (vec).y;                         \
    (mat).m33 = (vec).z;

// -----------------------------------------------------------------------------


extern const hmatrix IdentityHMatrix;
extern const matrix  IdentityMatrix;


void hmatMakeHMatFromMat(hmatrix *result, matrix *mat);
void hmatMakeHMatFromMatAndVec(hmatrix *result, matrix *mat, struct vector *pos);
void hmatCreateHMatFromHVecs(hmatrix *result, struct hvector *col1, struct hvector *col2, struct hvector *col3, struct hvector *col4);
void hmatMultiplyHMatByHMat(hmatrix *result, hmatrix *first, hmatrix *second);
void hmatMultiplyHMatByHVec(struct hvector *result, hmatrix *matrix, struct hvector *vector);
void hmatMultiplyHVecByHMat(struct hvector *result, struct hvector *vector, hmatrix *matrix);
void hmatTranspose(hmatrix *matrix);
void hmatCopyAndTranspose(hmatrix *src, hmatrix *dsttrans);
void hmatMakeRotAboutZ(hmatrix *matrix, real32 costheta, real32 sintheta);
void hmatMakeRotAboutX(hmatrix *matrix, real32 costheta, real32 sintheta);
void hmatMakeRotAboutY(hmatrix *matrix, real32 costheta, real32 sintheta);
void hmatPrintHMatrix(hmatrix *a);

void matGetMatFromHMat(matrix *result, hmatrix *hmat);
void matCreateCoordSysFromHeading(matrix *result, struct vector *heading);
void matCreateMatFromVecs(matrix *result, struct vector *col1, struct vector *col2, struct vector *col3);
void matMultiplyMatByMat(matrix *result, matrix *first, matrix *second);
void matMultiplyMatByVec(struct vector *result, matrix *matrix, struct vector *vector);
void matMultiplyVecByMat(struct vector *result, struct vector *vector, matrix *matrix);
void matTranspose(matrix *matrix);
void matCopyAndTranspose(matrix *src, matrix *dsttrans);
void matMakeRotAboutZ(matrix *matrix, real32 costheta, real32 sintheta);
void matMakeRotAboutX(matrix *matrix, real32 costheta, real32 sintheta);
void matMakeRotAboutY(matrix *matrix, real32 costheta, real32 sintheta);
void matPrintmatrix(matrix *a);
void matCopyAndScale(matrix *out, matrix *in, real32 scale);

#endif
