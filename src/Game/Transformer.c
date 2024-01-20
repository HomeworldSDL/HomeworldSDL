/*=============================================================================
    Name    : transformer.c
    Purpose : provides vertex buffer transformation

    Created 10/8/1998 by
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "Transformer.h"
#include "Memory.h"
#include "main.h"

void transformVertexList(sdword n, hvector* dest, vertexentry* source, hmatrix* m) {
    int i, j;

    for (i = 0; i < n; ++i) {
        dest[i].x = dest[i].y = dest[i].z = dest[i].w = 0.0;

        for (j = 0; j < 4; ++j) {
            dest[i].x += source[i].x * m->m11 + source[i].y * m->m21 + source[i].z * m->m31 + m->m41;
            dest[i].y += source[i].x * m->m12 + source[i].y * m->m22 + source[i].z * m->m32 + m->m42;
            dest[i].z += source[i].x * m->m13 + source[i].y * m->m23 + source[i].z * m->m33 + m->m43;
            dest[i].w += source[i].x * m->m14 + source[i].y * m->m24 + source[i].z * m->m34 + m->m44;
        }
    }
}

void transSingleTotalTransform(vector* screenSpace, hmatrix* modelview, hmatrix* projection, vector* worldSpace)
{
    hvector cameraSpace, screen;

    //world -> clip
    transformVertexList(1, &cameraSpace, (vertexentry*)worldSpace, modelview);
    cameraSpace.w = 1.0f;

    //clip -> screen
    hmatMultiplyHMatByHVec(&screen, projection, &cameraSpace);

    screenSpace->x = screen.x / screen.w;
    screenSpace->y = screen.y / screen.w;
}
