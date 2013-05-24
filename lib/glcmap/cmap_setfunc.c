
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void cmapSetPositionLightsFunc(CubeMap *cmap,
    void (*positionLights)(int view, void *context))
{
    cmap->positionLights = positionLights;
}

void cmapSetDrawViewFunc(CubeMap *cmap,
    void (*drawView)(int view, void *context))
{
    cmap->drawView = drawView;
}
