/*=============================================================================
    Name    : rglu.h
    Purpose : glu* functions for Homeworld

    Created 7/6/1998 by khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___RGLU_H
#define ___RGLU_H

#include "glinc.h"

void rgluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
GLint rgluProject(GLfloat objx, GLfloat objy, GLfloat objz,
                  GLfloat const* model, GLfloat const* proj,
                  GLint const* viewport,
                  GLfloat* winx, GLfloat* winy, GLfloat* winz);
GLint rgluUnProject(GLfloat winx, GLfloat winy, GLfloat winz,
                    GLfloat const* model, GLfloat const* proj,
                    GLint const* viewport,
                    GLfloat* objx, GLfloat* objy, GLfloat* objz);
void rgluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                GLfloat centerx, GLfloat centery, GLfloat centerz,
                GLfloat upx, GLfloat upy, GLfloat upz);
void rgluOrtho2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);
char* rgluErrorString(GLenum err);

#endif
