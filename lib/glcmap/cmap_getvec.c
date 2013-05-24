
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void
cmapGetEyeVector(CubeMap *cmap, GLfloat *eye)
{
    eye[X] = cmap->eye[X];
    eye[Y] = cmap->eye[Y];
    eye[Z] = cmap->eye[Z];
}

void
cmapGetUpVector(CubeMap *cmap, GLfloat *up)
{
    up[X] = cmap->up[X];
    up[Y] = cmap->up[Y];
    up[Z] = cmap->up[Z];
}

void
cmapGetObjectVector(CubeMap *cmap, GLfloat *obj)
{
    obj[X] = cmap->obj[X];
    obj[Y] = cmap->obj[Y];
    obj[Z] = cmap->obj[Z];
}

