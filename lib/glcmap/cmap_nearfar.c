
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void
cmapSetNearFar(CubeMap *cmap,
               GLfloat viewNear, GLfloat viewFar)
{
    /* Curse Intel for "near" and "far" keywords. */
    cmap->viewNear = viewNear;
    cmap->viewFar = viewFar;
}

void
cmapGetNearFar(CubeMap *cmap,
               GLfloat *viewNear, GLfloat *viewFar)
{
    /* Curse Intel for "near" and "far" keywords. */
        *viewNear = cmap->viewNear;
    *viewFar = cmap->viewFar;
}
