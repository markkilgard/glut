
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void cmapGetEye(CubeMap *cmap,
    GLfloat *eyex, GLfloat *eyey, GLfloat *eyez)
{
    *eyex = cmap->eye[X];
    *eyey = cmap->eye[Y];
    *eyez = cmap->eye[Z];
}

void cmapGetUp(CubeMap *cmap,
    GLfloat *upx, GLfloat *upy, GLfloat *upz)
{
    *upx = cmap->up[X];
    *upy = cmap->up[Y];
    *upz = cmap->up[Z];
}

void cmapGetObject(CubeMap *cmap,
    GLfloat *objx, GLfloat *objy, GLfloat *objz)
{
    *objx = cmap->obj[X];
    *objy = cmap->obj[Y];
    *objz = cmap->obj[Z];
}
