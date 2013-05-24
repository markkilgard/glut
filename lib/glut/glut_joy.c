
/* Copyright (c) Mark J. Kilgard, 1997, 1998, 2000. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#ifdef _WIN32
# include <windows.h>
# if !defined(__MINGW32__) && !defined(__CYGWIN32__)
#  include <mmsystem.h>  /* Win32 Multimedia API header. */
# else

/* Cygnus B20.1 does not contain a version of <mmsystem.h> so
   inline what we need. */
#define JOYERR_BASE            160
#define JOYERR_NOERROR        (0)                  /* no error */
#define JOYSTICKID1         0
#define JOY_RETURNX             0x00000001l
#define JOY_RETURNY             0x00000002l
#define JOY_RETURNZ             0x00000004l
#define JOY_RETURNR             0x00000008l
#define JOY_RETURNU             0x00000010l     /* axis 5 */
#define JOY_RETURNV             0x00000020l     /* axis 6 */
#define JOY_RETURNPOV           0x00000040l
#define JOY_RETURNBUTTONS       0x00000080l
#define JOY_RETURNALL           (JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | \
                                 JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | \
                                 JOY_RETURNPOV | JOY_RETURNBUTTONS)
typedef UINT MMRESULT;
typedef struct joyinfoex_tag {
    DWORD dwSize;                /* size of structure */
    DWORD dwFlags;               /* flags to indicate what to return */
    DWORD dwXpos;                /* x position */
    DWORD dwYpos;                /* y position */
    DWORD dwZpos;                /* z position */
    DWORD dwRpos;                /* rudder/4th axis position */
    DWORD dwUpos;                /* 5th axis position */
    DWORD dwVpos;                /* 6th axis position */
    DWORD dwButtons;             /* button states */
    DWORD dwButtonNumber;        /* current button number pressed */
    DWORD dwPOV;                 /* point of view state */
    DWORD dwReserved1;           /* reserved for communication between winmm & driver */
    DWORD dwReserved2;           /* reserved for future expansion */
} JOYINFOEX, FAR *LPJOYINFOEX;

MMRESULT WINAPI joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji);
MMRESULT WINAPI joyReleaseCapture(UINT uJoyID);
MMRESULT WINAPI joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged);
MMRESULT WINAPI joySetThreshold(UINT uJoyID, UINT uThreshold);

# endif
#endif

#include "glutint.h"

/* CENTRY */
void GLUTAPIENTRY
glutJoystickFunc(GLUTjoystickCB joystickFunc, int pollInterval)
{
#ifdef _WIN32
  if (joystickFunc && (pollInterval > 0)) {
    if (__glutCurrentWindow->entryState == WM_SETFOCUS) {
      MMRESULT result;

      /* Capture joystick focus if current window has
         focus now. */
      result = joySetCapture(__glutCurrentWindow->win,
        JOYSTICKID1, 0, TRUE);
      if (result == JOYERR_NOERROR) {
        (void) joySetThreshold(JOYSTICKID1, pollInterval);
      }
    }
    __glutCurrentWindow->joyPollInterval = pollInterval;
  } else {
    /* Release joystick focus if current window has
       focus now. */
    if (__glutCurrentWindow->joystick
      && (__glutCurrentWindow->joyPollInterval > 0)
      && (__glutCurrentWindow->entryState == WM_SETFOCUS)) {
      (void) joyReleaseCapture(JOYSTICKID1);
    }
    __glutCurrentWindow->joyPollInterval = 0;
  }
  __glutCurrentWindow->joystick = joystickFunc;
#else
  /* XXX No support currently for X11 joysticks. */
#endif
}

void GLUTAPIENTRY
glutForceJoystickFunc(void)
{
#ifdef _WIN32
  if (__glutCurrentWindow->joystick) {
    JOYINFOEX jix;
    MMRESULT res;
    int x, y, z;

    /* Poll the joystick. */
    jix.dwSize = sizeof(jix);
    jix.dwFlags = JOY_RETURNALL;
    res = joyGetPosEx(JOYSTICKID1,&jix);
    if (res == JOYERR_NOERROR) {

      /* Convert to int for scaling. */
      x = jix.dwXpos;
      y = jix.dwYpos;
      z = jix.dwZpos;

#define SCALE(v)  ((int) ((v - 32767)/32.768))

      __glutCurrentWindow->joystick(jix.dwButtons,
        SCALE(x), SCALE(y), SCALE(z));
    }
  }
#else
  /* XXX No support currently for X11 joysticks. */
#endif
}

/* ENDCENTRY */
