
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include "glcmapint.h"

void
cmapSetFlags(CubeMap *cmap, CubeMapFlags flags)
{
    cmap->flags = flags;
}

void
cmapGetFlags(CubeMap *cmap, CubeMapFlags *flags)
{
    *flags = cmap->flags;
}
