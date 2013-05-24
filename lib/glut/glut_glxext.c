
/* Copyright (c) Mark J. Kilgard, 1997, 2001. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include <stdlib.h>
#include <string.h>
#include "glutint.h"

/* Some Mesa versions define GLX_VERSION_1_2 without defining
   GLX_VERSION_1_1. */
#if defined(GLX_VERSION_1_2) && !defined(GLX_VERSION_1_1)
# define GLX_VERSION_1_1 1
#endif

int
__glutIsSupportedByGLX(const char *extension)
{
#if defined(GLX_VERSION_1_1)
  static const char *extensions = NULL;
  const char *start;
  char *where, *terminator;
  int major, minor;

  glXQueryVersion(__glutDisplay, &major, &minor);
  /* Be careful not to call glXQueryExtensionsString if it
     looks like the server doesn't support GLX 1.1.
     Unfortunately, the original GLX 1.0 didn't have the notion
     of GLX extensions. */
  if ((major == 1 && minor >= 1) || (major > 1)) {
    if (!extensions) {
      extensions = glXQueryExtensionsString(__glutDisplay, __glutScreen);
    }
    /* It takes a bit of care to be fool-proof about parsing
       the GLX extensions string.  Don't be fooled by
       sub-strings,  etc. */
    start = extensions;
    for (;;) {
      where = strstr(start, extension);
      if (!where) {
        return 0;
      }
      terminator = where + strlen(extension);
      if (where == start || *(where - 1) == ' ') {
        if (*terminator == ' ' || *terminator == '\0') {
          return 1;
        }
      }
      start = terminator;
    }
  }
#endif
  return 0;
}
