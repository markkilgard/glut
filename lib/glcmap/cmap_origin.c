
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include <GL/glcmap.h>

#include "glcmapint.h"

void cmapSetViewOrigin(CubeMap *cmap, GLint x, GLint y)
{
    cmap->viewOrigin[0] = x;
    cmap->viewOrigin[1] = y;
}

void cmapSetCubeMapOrigin(CubeMap *cmap, GLint x, GLint y)
{
    cmap->cmapOrigin[0] = x;
    cmap->cmapOrigin[1] = y;
}

void cmapGetViewOrigin(CubeMap *cmap, GLint *x, GLint *y)
{
    *x = cmap->viewOrigin[0];
    *y = cmap->viewOrigin[1];
}

void cmapGetCubeMapOrigin(CubeMap *cmap, GLint *x, GLint *y)
{
    *x = cmap->cmapOrigin[0];
    *y = cmap->cmapOrigin[1];
}
