
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void cmapSetEye(CubeMap *cmap,
    GLfloat eyex, GLfloat eyey, GLfloat eyez)
{
    cmap->eye[X] = eyex;
    cmap->eye[Y] = eyey;
    cmap->eye[Z] = eyez;
}

void cmapSetUp(CubeMap *cmap,
    GLfloat upx, GLfloat upy, GLfloat upz)
{
    cmap->up[X] = upx;
    cmap->up[Y] = upy;
    cmap->up[Z] = upz;
}

void cmapSetObject(CubeMap *cmap,
    GLfloat objx, GLfloat objy, GLfloat objz)
{
    cmap->obj[X] = objx;
    cmap->obj[Y] = objy;
    cmap->obj[Z] = objz;
}
