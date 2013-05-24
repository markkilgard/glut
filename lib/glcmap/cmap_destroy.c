
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include <stdio.h>  /* SunOS multithreaded assert() needs <stdio.h>.  Lame. */
#include <assert.h>
#include <stdlib.h>

#include "glcmapint.h"

void
cmapDestroyCubeMap(CubeMap *cmap)
{
    free(cmap);
}
