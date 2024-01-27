/*=============================================================================
    Name    : transformer.h
    Purpose : provides vertex buffer transformation

    Created 10/8/1998 by
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___TRANSFORMER_H
#define ___TRANSFORMER_H

#include "Matrix.h"
#include "Mesh.h"
#include "Types.h"
#include "Vector.h"

void transSingleTotalTransform(vector* screenSpace, hmatrix* modelview, hmatrix* projection, vector* worldSpace);

#endif
