
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include <GL/glcmap.h>
#include <stdlib.h>

#include "glcmapint.h"

CubeMap *
cmapCreateCubeMap(CubeMap *shareSmap)
{
    CubeMap *cmap;

    cmap = (CubeMap*) malloc(sizeof(CubeMap));

    /* Default texture objects. */
    cmap->cmapTexObj = 1001;

    /* Default texture dimensions 64x64 */
    cmap->viewTexDim = 64;
    cmap->cmapTexDim = 64;

    /* Default origin at lower left. */
    cmap->viewOrigin[X] = 0;
    cmap->viewOrigin[Y] = 0;
    cmap->cmapOrigin[X] = 0;
    cmap->cmapOrigin[Y] = 0;

    /* Flags. */
    cmap->flags = (CubeMapFlags) 0;

    /* Default eye vector. */
    cmap->eye[X] = 0.0;
    cmap->eye[Y] = 0.0;
    cmap->eye[Z] = -10.0;

    /* Default up vector. */
    cmap->up[X] = 0.0;
    cmap->up[Y] = 0.1;
    cmap->up[Z] = 0.0;

    /* Default object location vector. */
    cmap->obj[X] = 0.0;
    cmap->obj[Y] = 0.0;
    cmap->obj[Z] = 0.0;

    /* Default near and far clip planes. */
    cmap->viewNear = 0.1;
    cmap->viewFar = 20.0;

    cmap->positionLights = NULL;
    cmap->drawView = NULL;

    cmap->context = NULL;

    return cmap;
}
