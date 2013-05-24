
/* Copyright (c) Nate Robins, 1997. */
/* portions Copyright (c) Mark Kilgard, 1997, 1998, 2000. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */


#include "glutint.h"
#include "glutstroke.h"
#include "glutbitmap.h"

#if defined(__MINGW32)
typedef MINMAXINFO *LPMINMAXINFO;
#endif

extern StrokeFontRec glutStrokeRoman, glutStrokeMonoRoman;
extern BitmapFontRec glutBitmap8By13, glutBitmap9By15, glutBitmapTimesRoman10, glutBitmapTimesRoman24, glutBitmapHelvetica10, glutBitmapHelvetica12, glutBitmapHelvetica18;

/* Strange, but "ftime" appears to be faster on both Windows 98 and
   Windows NT 4.0 than using "GetSystemTime" to return a timeval.
   So just keep using ftime, but the "GetSystemTime" code is here
   for reference. */
#define USE_GetSystemTime 0

#if USE_GetSystemTime && defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64
/*
 * Number of clock ticks per second. A clock tick is the unit by which
 * processor time is measured and is returned by 'clock'.
 */
#define CLOCKS_PER_SEC  1000.0
#define FACTOR (0x19db1ded53ea710ui64)
#define NSPERSEC 10000000ui64

static unsigned __int64
__to_clock_t (FILETIME * src)
{
  unsigned __int64 total = ((unsigned __int64) src->dwHighDateTime << 32) + ((unsigned)src->dwLowDateTime);

  /* Convert into clock ticks - the total is in 10ths of a usec.  */
  total -= FACTOR;

  total /= (unsigned __int64) (NSPERSEC / CLOCKS_PER_SEC);
  return total;
}

static void
totimeval (struct timeval *dst, FILETIME *src)
{
  __int64 x = __to_clock_t (src);

  x *= (int) (1e6) / CLOCKS_PER_SEC; /* Turn x into usecs */

  dst->tv_usec = x % (__int64) (1e6); /* And split */
  dst->tv_sec = x / (__int64) (1e6);
}

int
gettimeofdayWIN32(struct timevalWIN32* tp)
{
  SYSTEMTIME t;
  FILETIME f;

  GetSystemTime (&t);
  if (!SystemTimeToFileTime (&t, &f)) {
    /* GLUT never expects this to fail! */
    return -1;
  }
  totimeval (tp, &f);

  /* 0 indicates that the call succeeded. */
  return 0;
}

#else /* no 64-bit data type support so use mundane ftime based gettimeofday */

#if defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64
/* GetSystemTimeAsFileTime has lower latency and more precision (but
   apparently no finer granularity) than ftime. */
int
gettimeofdayWIN32(struct timevalWIN32* tp)
{
  union {
    LONGLONG ns100; /*time since 1 Jan 1601 in 100ns units */
    FILETIME ft;
  } now;

  GetSystemTimeAsFileTime (&now.ft);
  tp->tv_usec = (long) ((now.ns100 / 10i64) % 1000000i64);
  tp->tv_sec = (long) ((now.ns100 - 116444736000000000i64) / 10000000i64);
  return 0;
} 
#else
#include <sys/timeb.h>  /* For the ftime prototype. */
int
gettimeofdayWIN32(struct timevalWIN32* tp)
{
  struct timeb tb;

  ftime(&tb);
  tp->tv_sec = tb.time;
  tp->tv_usec = tb.millitm * 1000;

  /* 0 indicates that the call succeeded. */
  return 0;
}
#endif

#endif

#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4715)
#endif

/* To get around the fact that Microsoft DLLs only allow functions
   to be exported and now data addresses (as Unix DSOs support), the
   GLUT API constants such as GLUT_STROKE_ROMAN have to get passed
   through a case statement to get mapped to the actual data structure
   address. */
void*
__glutFont(void *font)
{
  switch((UINT_PTR)font) {
  case (UINT_PTR)GLUT_STROKE_ROMAN:
    return &glutStrokeRoman;
  case (UINT_PTR)GLUT_STROKE_MONO_ROMAN:
    return &glutStrokeMonoRoman;
  case (UINT_PTR)GLUT_BITMAP_9_BY_15:
    return &glutBitmap9By15;
  case (UINT_PTR)GLUT_BITMAP_8_BY_13:
    return &glutBitmap8By13;
  case (UINT_PTR)GLUT_BITMAP_TIMES_ROMAN_10:
    return &glutBitmapTimesRoman10;
  case (UINT_PTR)GLUT_BITMAP_TIMES_ROMAN_24:
    return &glutBitmapTimesRoman24;
  case (UINT_PTR)GLUT_BITMAP_HELVETICA_10:
    return &glutBitmapHelvetica10;
  case (UINT_PTR)GLUT_BITMAP_HELVETICA_12:
    return &glutBitmapHelvetica12;
  case (UINT_PTR)GLUT_BITMAP_HELVETICA_18:
    return &glutBitmapHelvetica18;
  }
  __glutFatalError("out of memory.");
  /* NOTREACHED */
}
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif

int
__glutGetTransparentPixel(Display * dpy, XVisualInfo * vinfo)
{
  /* the transparent pixel on Win32 is always index number 0.  So if
     we put this routine in this file, we can avoid compiling the
     whole of layerutil.c which is where this routine normally comes
     from. */
  return 0;
}

void
__glutAdjustCoords(Window parent, int* x, int* y, int* width, int* height)
{
  RECT rect;

  /* adjust the window rectangle because Win32 thinks that the x, y,
     width & height are the WHOLE window (including decorations),
     whereas GLUT treats the x, y, width & height as only the CLIENT
     area of the window. */
  rect.left = *x; rect.top = *y;
  rect.right = *x + *width; rect.bottom = *y + *height;

  /* must adjust the coordinates according to the correct style
     because depending on the style, there may or may not be
     borders. */
  AdjustWindowRect(&rect, WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                   (parent ? WS_CHILD : WS_OVERLAPPEDWINDOW),
                   FALSE);
  /* FALSE in the third parameter = window has no menu bar */

  /* readjust if the x and y are offscreen */
  if(rect.left < 0) {
    *x = 0;
  } else {
    *x = rect.left;
  }
  
  if(rect.top < 0) {
    *y = 0;
  } else {
    *y = rect.top;
  }

  *width = rect.right - rect.left;      /* adjusted width */
  *height = rect.bottom - rect.top;     /* adjusted height */
}

