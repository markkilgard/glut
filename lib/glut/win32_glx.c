
/* Copyright (c) Nate Robins, 1997. */
/* Copyright (c) Mark J. Kilgard, 2001. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include <assert.h>

#include "glutint.h"

/* global current HDC */
extern HDC XHDC;

int
glXGetConfig(Display * display, XVisualInfo * visual, int attrib, int *value)
{
  DWORD capableMask;

  if (!visual) {
    return GLX_BAD_VISUAL;
  }

  switch (attrib) {
  case GLX_USE_GL:
    /* If ARB_pixel_format is supported, use it to query the "support
       OpenGL" and "draw to window" booleans for the given PFD.  This is
       because multisample visuals fail "conform" because of multisample
       rendering variances.  To avoid conform failures, IHVs should
       advertise multisample PFDS _without_ the PFD_SUPPORT_OPENGL bit.
       OpenGL applications that expect to use multisampling must use
       the ARB_pixel_format API to get at the true "support OpenGL"
       information.  */
    if (wglGetExtensionsStringARB &&
        has_WGL_ARB_extensions_string &&
        has_WGL_ARB_pixel_format &&
        wglGetPixelFormatAttribivARB &&
        wglGetPixelFormatAttribfvARB) {

        static const int attribs[] = {
          WGL_DRAW_TO_WINDOW_ARB,
          WGL_SUPPORT_OPENGL_ARB,
          WGL_PIXEL_TYPE_ARB,
          WGL_COLOR_BITS_ARB };
        int attribValues[4];
        BOOL rc;

        assert(sizeof(attribs) == sizeof(attribValues));

        rc = wglGetPixelFormatAttribivARB(XHDC, visual->num,
          0, /* layer zero */
          sizeof(attribs)/sizeof(attribs[0]),
          attribs, attribValues);
        if (rc == 1) {
          if ((attribValues[0] == 1) && (attribValues[1] == 1)) {
            /* Ignore color index windows with too many color bits (>=24). */
            if ((attribValues[2] == WGL_TYPE_COLORINDEX_ARB) && (attribValues[3] >= 24)) {
              *value = 0;
            } else {
              *value = 1;
            }
          } else {
            *value = 0;
          }
          break;
        }
        /* wglGetPixelFormatAttribivARB is not performing exactly as we
           expect so continue on the normal way...  */
    }
    capableMask = visual->pfd.dwFlags & (PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW);
    if (capableMask == (PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW)) {
      /* XXX Brad's Matrix Millenium II has problems creating
         color index windows in 24-bit mode (leads to GDI crash)
         and 32-bit mode (leads to black window).  The cColorBits
         field of the PIXELFORMATDESCRIPTOR returned claims to
         have 24 and 32 bits respectively of color indices. 2^24
         and 2^32 are ridiculously huge writable colormaps.
         Assume that if we get back a color index
         PIXELFORMATDESCRIPTOR with 24 or more bits, the
         PIXELFORMATDESCRIPTOR doesn't really work and skip it.
         -mjk */
      if (visual->pfd.iPixelType == PFD_TYPE_COLORINDEX
        && visual->pfd.cColorBits >= 24) {
        *value = 0;
      } else {
        *value = 1;
      }
    } else {
      *value = 0;
    }
    break;
  case GLX_BUFFER_SIZE:
    /* KLUDGE: if we're RGBA, return the number of bits/pixel,
       otherwise, return 8 (we guessed at 256 colors in CI
       mode). */
    if (visual->pfd.iPixelType == PFD_TYPE_RGBA) {
      *value = visual->pfd.cColorBits;
    } else {
      *value = 8;
    }
    break;
  case GLX_LEVEL:
    /* The bReserved flag of the pfd contains the
       overlay/underlay info. */
    *value = visual->pfd.bReserved;
    break;
  case GLX_RGBA:
    *value = visual->pfd.iPixelType == PFD_TYPE_RGBA;
    break;
  case GLX_DOUBLEBUFFER:
    *value = visual->pfd.dwFlags & PFD_DOUBLEBUFFER;
    break;
  case GLX_STEREO:
    *value = visual->pfd.dwFlags & PFD_STEREO;
    break;
  case GLX_AUX_BUFFERS:
    *value = visual->pfd.cAuxBuffers;
    break;
  case GLX_RED_SIZE:
    *value = visual->pfd.cRedBits;
    break;
  case GLX_GREEN_SIZE:
    *value = visual->pfd.cGreenBits;
    break;
  case GLX_BLUE_SIZE:
    *value = visual->pfd.cBlueBits;
    break;
  case GLX_ALPHA_SIZE:
    *value = visual->pfd.cAlphaBits;
    break;
  case GLX_DEPTH_SIZE:
    *value = visual->pfd.cDepthBits;
    break;
  case GLX_STENCIL_SIZE:
    *value = visual->pfd.cStencilBits;
    break;
  case GLX_ACCUM_RED_SIZE:
    *value = visual->pfd.cAccumRedBits;
    break;
  case GLX_ACCUM_GREEN_SIZE:
    *value = visual->pfd.cAccumGreenBits;
    break;
  case GLX_ACCUM_BLUE_SIZE:
    *value = visual->pfd.cAccumBlueBits;
    break;
  case GLX_ACCUM_ALPHA_SIZE:
    *value = visual->pfd.cAccumAlphaBits;
    break;
  case GLX_VISUAL_CAVEAT_EXT:
    if ((visual->pfd.dwFlags & PFD_GENERIC_FORMAT) &&
        !(visual->pfd.dwFlags & PFD_GENERIC_ACCELERATED)) {

      /* If both the PFD_GENERIC_ACCELERATED and PFD_GENERIC_FORMAT 
         flags are set, the PFD is accelerated by a Mini Client Driver (MCD),
         but if PFD_GENERIC_FORMAT is set without PFD_GENERIC_ACCELERATED,
         that means Microsoft's slow software renderer handles the PFD.
         We always consider such PFDs "slow". */

      *value = GLX_SLOW_VISUAL_EXT;
    } else {
      *value = GLX_NONE_EXT;
    }
    break;
  case WGL_SAMPLES_ARB:
    if (wglGetExtensionsStringARB &&
        has_WGL_ARB_extensions_string &&
        has_WGL_ARB_pixel_format &&
        wglGetPixelFormatAttribivARB &&
        wglGetPixelFormatAttribfvARB &&
        has_GL_ARB_multisample &&
        has_WGL_ARB_multisample &&
        glSampleCoverageARB) {

        static const int attribs[] = { WGL_SAMPLES_ARB };
        BOOL rc;

        rc = wglGetPixelFormatAttribivARB(XHDC, visual->num, 0, 1, attribs, value);
        if (rc == 0) {
          *value = 0;
        }
    } else {
       *value = 0;
    }
    break;
  default:
    return GLX_BAD_ATTRIB;
  }
  return 0;
}

static XVisualInfo *
pfdToXVisualInfo(int pf)
{
  XVisualInfo *match;

  if (pf > 0) {
    match = (XVisualInfo *) malloc(sizeof(XVisualInfo));
    if (match == NULL) {
      __glutFatalError("out of memory.");
    }
    DescribePixelFormat(XHDC, pf, sizeof(XVisualInfo), &match->pfd);

    /* XXX Brad's Matrix Millenium II has problems creating
       color index windows in 24-bit mode (lead to GDI crash)
       and 32-bit mode (lead to black window).  The cColorBits
       filed of the PIXELFORMATDESCRIPTOR returned claims to
       have 24 and 32 bits respectively of color indices. 2^24
       and 2^32 are ridiculously huge writable colormaps.
       Assume that if we get back a color index
       PIXELFORMATDESCRIPTOR with 24 or more bits, the
       PIXELFORMATDESCRIPTOR doesn't really work and skip it.
       -mjk */
    if (match->pfd.iPixelType == PFD_TYPE_COLORINDEX &&
        match->pfd.cColorBits >= 24) {
      free(match);
      return NULL;
    }

    match->num = pf;
  } else {
    match = NULL;
  }
  return match;
}

static XVisualInfo *
chooseMultisampleVisual(Display * display, int screen, int *attribList)
{
  int *p = attribList;
  int newAttribList[70];
  FLOAT emptyAttribList = 0;
  int n = 0;
  XVisualInfo *match = NULL;
  int hasRGBA = 0;
  int hasDoubleBuffer = 0;
  int hasStereo = 0;
  BOOL worked;
  int pf;
  UINT numFormats;

  assert(wglChoosePixelFormatARB);

  /* Mandatory attributes. */
  newAttribList[n++] = WGL_ACCELERATION_ARB;
  newAttribList[n++] = WGL_FULL_ACCELERATION_ARB;  /* MUST BE SECOND ENTRY */
  newAttribList[n++] = WGL_DRAW_TO_WINDOW_ARB;
  newAttribList[n++] = 1;
  newAttribList[n++] = WGL_NUMBER_OVERLAYS_ARB;
  newAttribList[n++] = 0;
  newAttribList[n++] = WGL_NUMBER_UNDERLAYS_ARB;
  newAttribList[n++] = 0;

  while (*p) {
    switch (*p) {
    case GLX_RGBA:
      hasRGBA = 1;
      newAttribList[n++] = WGL_PIXEL_TYPE_ARB;
      newAttribList[n++] = WGL_TYPE_RGBA_ARB;
      break;
    case GLX_USE_GL:
      newAttribList[n++] = WGL_SUPPORT_OPENGL_ARB;
      newAttribList[n++] = 1;
      break;
    case GLX_DOUBLEBUFFER:
      hasDoubleBuffer = 1;
      newAttribList[n++] = WGL_DOUBLE_BUFFER_ARB;
      newAttribList[n++] = 1;
      break;
    case GLX_STEREO:
      hasStereo = 1;
      newAttribList[n++] = WGL_STEREO_ARB;
      newAttribList[n++] = 1;
      break;
    case GLX_BUFFER_SIZE:
      newAttribList[n++] = WGL_COLOR_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_LEVEL:
      /* Ignore the level in Win32; Win32 GLUT lacks overlay support. */
      p++;  /* Skip the parameter. */
      break;
    case GLX_AUX_BUFFERS:
      newAttribList[n++] = WGL_AUX_BUFFERS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_RED_SIZE:
      newAttribList[n++] = WGL_RED_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_GREEN_SIZE:
      newAttribList[n++] = WGL_GREEN_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_BLUE_SIZE:
      newAttribList[n++] = WGL_BLUE_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_ALPHA_SIZE:
      newAttribList[n++] = WGL_ALPHA_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_DEPTH_SIZE:
      newAttribList[n++] = WGL_DEPTH_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_STENCIL_SIZE:
      newAttribList[n++] = WGL_STENCIL_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_ACCUM_RED_SIZE:
      newAttribList[n++] = WGL_ACCUM_RED_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_ACCUM_GREEN_SIZE:
      newAttribList[n++] = WGL_ACCUM_GREEN_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_ACCUM_BLUE_SIZE:
      newAttribList[n++] = WGL_ACCUM_BLUE_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;
    case GLX_ACCUM_ALPHA_SIZE:
      newAttribList[n++] = WGL_ACCUM_ALPHA_BITS_ARB;
      newAttribList[n++] = *(++p);
      break;

/* GLX_SAMPLES_SGIS has the same value as GLX_SAMPLES_ARB. */
# ifndef GLX_SAMPLES_ARB
#  define GLX_SAMPLES_ARB                      100001
# endif

    case GLX_SAMPLES_ARB:
      newAttribList[n++] = WGL_SAMPLES_ARB;
      newAttribList[n++] = *(++p);
      break;
    }
    p++;
  }

  if (!hasRGBA) {
    newAttribList[n++] = WGL_PIXEL_TYPE_ARB;
    newAttribList[n++] = WGL_TYPE_COLORINDEX_ARB;
  }
  if (!hasDoubleBuffer) {
    newAttribList[n++] = WGL_DOUBLE_BUFFER_ARB;
    newAttribList[n++] = 0;
  }
  if (!hasStereo) {
    newAttribList[n++] = WGL_STEREO_ARB;
    newAttribList[n++] = 0;
  }

  /* Terminate the attribute list with a zero. */
  newAttribList[n++] = 0;

  assert((p - attribList) < sizeof(newAttribList)/sizeof(newAttribList[0]));

  /* Attempt to get FULL_ACCELERATION (ICD support), GENERIC_ACCELERATION
     (MCD support), and NO_ACCELERATION in that order. */

  worked = wglChoosePixelFormatARB(XHDC, newAttribList,
    &emptyAttribList,  /* Should be able to pass NULL but early NVIDIA drivers crash. */
    1,     /* Just return one format. */
    &pf,
    &numFormats);

  if (!worked || numFormats <= 0) {
    assert(newAttribList[0] == WGL_ACCELERATION_ARB);
    newAttribList[1] = WGL_GENERIC_ACCELERATION_ARB;

    worked = wglChoosePixelFormatARB(XHDC, newAttribList,
      &emptyAttribList,  /* Should be able to pass NULL but early NVIDIA drivers crash. */
      1,     /* Just return one format. */
      &pf,
      &numFormats);

    if (!worked || numFormats <= 0) {
      newAttribList[1] = WGL_NO_ACCELERATION_ARB;

      worked = wglChoosePixelFormatARB(XHDC, newAttribList,
        &emptyAttribList,  /* Should be able to pass NULL but early NVIDIA drivers crash. */
        1,     /* Just return one format. */
        &pf,
        &numFormats);

      return NULL;  /* Every attempt failed. */
    }
  }

  match = pfdToXVisualInfo(pf);

  return match;
}

XVisualInfo *
glXChooseVisual(Display * display, int screen, int *attribList)
{
  int *p = attribList;
  int pf;
  PIXELFORMATDESCRIPTOR pfd;
  XVisualInfo *match;
  int stereo = 0;
  int alpha = 0;

  assert(XHDC);

  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nSize = (sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nVersion = 1;

  /* Defaults. */
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd.iPixelType = PFD_TYPE_COLORINDEX;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;

  while (*p) {
    switch (*p) {
    case GLX_USE_GL:
      pfd.dwFlags |= PFD_SUPPORT_OPENGL;
      break;
    case GLX_BUFFER_SIZE:
      pfd.cColorBits = *(++p);
      break;
    case GLX_LEVEL:
      /* the bReserved flag of the pfd contains the
         overlay/underlay info. */
      pfd.bReserved = *(++p);
      break;
    case GLX_RGBA:
      pfd.iPixelType = PFD_TYPE_RGBA;
      break;
    case GLX_DOUBLEBUFFER:
      pfd.dwFlags |= PFD_DOUBLEBUFFER;
      break;
    case GLX_STEREO:
      stereo = 1;
      pfd.dwFlags |= PFD_STEREO;
      break;
    case GLX_AUX_BUFFERS:
      pfd.cAuxBuffers = *(++p);
      break;
    case GLX_RED_SIZE:
      pfd.cRedBits = 8; /* Try to get the maximum. */
      p++;
      break;
    case GLX_GREEN_SIZE:
      pfd.cGreenBits = 8;
      p++;
      break;
    case GLX_BLUE_SIZE:
      pfd.cBlueBits = 8;
      p++;
      break;
    case GLX_ALPHA_SIZE:
      alpha = 1;
      pfd.cAlphaBits = 8;
      p++;
      break;
    case GLX_DEPTH_SIZE:
      pfd.cDepthBits = 32;
      p++;
      break;
    case GLX_STENCIL_SIZE:
      pfd.cStencilBits = *(++p);
      break;
    case GLX_ACCUM_RED_SIZE:
    case GLX_ACCUM_GREEN_SIZE:
    case GLX_ACCUM_BLUE_SIZE:
    case GLX_ACCUM_ALPHA_SIZE:
      /* I believe that WGL only used the cAccumRedBits,
         cAccumBlueBits, cAccumGreenBits, and cAccumAlphaBits fields
         when returning info about the accumulation buffer precision.
         Only cAccumBits is used for requesting an accumulation
         buffer. */
      pfd.cAccumBits = 1;
      p++;
      break;
    case GLX_SAMPLES_ARB:
      if (wglGetExtensionsStringARB &&
          has_WGL_ARB_extensions_string &&
          has_WGL_ARB_pixel_format &&
          wglGetPixelFormatAttribivARB &&
          wglGetPixelFormatAttribfvARB &&
          has_GL_ARB_multisample &&
          has_WGL_ARB_multisample &&
          glSampleCoverageARB) {
        return chooseMultisampleVisual(display, screen, attribList);
      }
      p++;
      break;
    }
    p++;
  }

  /* Let Win32 choose one for us. */
  pf = ChoosePixelFormat(XHDC, &pfd);

  match = pfdToXVisualInfo(pf);
  if (match) {
    /* ChoosePixelFormat is dumb in that it will return a pixel
       format that doesn't have stereo even if it was requested
       so we need to make sure that if stereo was selected, we
       got it. */
    if (stereo) {
      if (!(match->pfd.dwFlags & PFD_STEREO)) {
        free(match);
        return NULL;
      }
    }

    /* Same applies to alpha. */
    if (alpha) {
      if (match->pfd.cAlphaBits == 0) {
        free(match);
        return NULL;
      }
    }
  }
  return match;
}
