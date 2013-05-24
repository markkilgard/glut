
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void
cmapGetCubeMapTexObj(CubeMap *cmap, GLuint *texobj)
{
    *texobj = cmap->cmapTexObj;
}

void
cmapGetViewTexObj(CubeMap *cmap, GLuint *texobj)
{
    *texobj = cmap->viewTexObj;
}

void
cmapGetViewTexObjs(CubeMap *cmap, GLuint texobjs[6])
{
    int i;

    for (i=0; i<6; i++) {
        texobjs[i] = cmap->viewTexObjs[i];
    }
}

