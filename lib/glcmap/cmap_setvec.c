
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void
cmapSetEyeVector(CubeMap *cmap, GLfloat *eye)
{
    cmap->eye[X] = eye[X];
    cmap->eye[Y] = eye[Y];
    cmap->eye[Z] = eye[Z];
}

void
cmapSetUpVector(CubeMap *cmap, GLfloat *up)
{
    cmap->up[X] = up[X];
    cmap->up[Y] = up[Y];
    cmap->up[Z] = up[Z];
}

void
cmapSetObjectVector(CubeMap *cmap, GLfloat *obj)
{
    cmap->obj[X] = obj[X];
    cmap->obj[Y] = obj[Y];
    cmap->obj[Z] = obj[Z];
}

