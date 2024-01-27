/*=============================================================================
    Name    : transformer.c
    Purpose : provides vertex buffer transformation

    Created 10/8/1998 by
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "Transformer.h"
#include "Memory.h"
#include "main.h"

void transSingleTotalTransform(vector* screenSpace, hmatrix* modelview, hmatrix* projection, vector* worldSpace)
{
    hvector cameraSpace, screen, point;

    point.x = worldSpace->x;
    point.y = worldSpace->y;
    point.z = worldSpace->z;
    point.w = 1.0f;

    //world -> clip
    hmatMultiplyHMatByHVec(&cameraSpace, modelview, &point);

    //clip -> screen
    hmatMultiplyHMatByHVec(&screen, projection, &cameraSpace);

    screenSpace->x = screen.x / screen.w;
    screenSpace->y = screen.y / screen.w;
}
