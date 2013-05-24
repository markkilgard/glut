#ifndef __glcmap_h__
#define __glcmap_h__

/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#if defined(_WIN32)

/* Try hard to avoid including <windows.h> to avoid name space pollution,
   but Win32's <GL/gl.h> needs APIENTRY and WINGDIAPI defined properly. */
# if 0
#  define  WIN32_LEAN_AND_MEAN
#  include <windows.h>
# else
   /* XXX This is from Win32's <windef.h> */
#  ifndef APIENTRY
#   if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#    define APIENTRY    __stdcall
#   else
#    define APIENTRY
#   endif
#  endif
#  ifndef CALLBACK
    /* XXX This is from Win32's <winnt.h> */
#   if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#   else
#    define CALLBACK
#   endif
#  endif
   /* XXX This is from Win32's <wingdi.h> and <winnt.h> */
#  ifndef WINGDIAPI
#   define WINGDIAPI __declspec(dllimport)
#  endif
   /* XXX This is from Win32's <ctype.h> */
#  ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#   define _WCHAR_T_DEFINED
#  endif
# endif

#pragma warning (disable:4244)  /* Disable bogus conversion warnings. */
#pragma warning (disable:4305)  /* VC++ 5.0 version of above warning. */

#endif /* _WIN32 */

#if defined(__APPLE__) && defined(__MACH__)
/* Mac OS X places GL headers within OpenGL framework. */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
/* All other platforms put GL headers in the standard place. */
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
        CMAP_CLEAR_CMAP_TEXTURE = 0x1,
        CMAP_GENERATE_MIPMAPS = 0x2
} CubeMapFlags;

/* Cube view enumerants. */
enum {
        CMAP_FRONT = 0,
        CMAP_TOP = 1,
        CMAP_BOTTOM = 2,
        CMAP_LEFT = 3,
        CMAP_RIGHT = 4,
        CMAP_BACK = 5
};

typedef struct _CubeMap CubeMap;

extern CubeMap *cmapCreateCubeMap(CubeMap *shareSmap);
extern void cmapDestroyCubeMap(CubeMap *cmap);

extern void cmapConfigureCubeMapMesh(CubeMap *cmap, int steps, int rings, int edgeExtend);

extern void cmapSetCubeMapTexObj(CubeMap *cmap, GLuint texobj);
extern void cmapGetCubeMapTexObj(CubeMap *cmap, GLuint *texobj);
extern void cmapGetViewTexObj(CubeMap *cmap, GLuint *texobj);
extern void cmapGetViewTexObjs(CubeMap *cmap, GLuint texobjs[6]);

extern void cmapSetFlags(CubeMap *cmap, CubeMapFlags flags);
extern void cmapGetFlags(CubeMap *cmap, CubeMapFlags *flags);

extern void cmapSetViewOrigin(CubeMap *cmap, GLint x, GLint y);
extern void cmapSetCubeMapOrigin(CubeMap *cmap, GLint x, GLint y);
extern void cmapGetViewOrigin(CubeMap *cmap, GLint *x, GLint *y);
extern void cmapGetCubeMapOrigin(CubeMap *cmap, GLint *x, GLint *y);

extern void cmapSetEye(CubeMap *cmap, GLfloat eyex, GLfloat eyey,     GLfloat eyez);
extern void cmapSetEyeVector(CubeMap *cmap, GLfloat *eye);
extern void cmapSetUp(CubeMap *cmap, GLfloat upx, GLfloat upy, GLfloat upz);
extern void cmapSetUpVector(CubeMap *cmap, GLfloat *up);
extern void cmapSetObject(CubeMap *cmap, GLfloat objx, GLfloat objy, GLfloat objz);
extern void cmapSetObjectVector(CubeMap *cmap, GLfloat *obj);
extern void cmapGetEye(CubeMap *cmap, GLfloat *eyex, GLfloat *eyey, GLfloat *eyez);
extern void cmapGetEyeVector(CubeMap *cmap, GLfloat *eye);
extern void cmapGetUp(CubeMap *cmap, GLfloat *upx, GLfloat *upy, GLfloat *upz);
extern void cmapGetUpVector(CubeMap *cmap, GLfloat *up);
extern void cmapGetObject(CubeMap *cmap, GLfloat *objx, GLfloat *objy, GLfloat *objz);
extern void cmapGetObjectVector(CubeMap *cmap, GLfloat *obj);

extern void cmapSetNearFar(CubeMap *cmap, GLfloat viewNear, GLfloat viewFar);
extern void cmapGetNearFar(CubeMap *cmap, GLfloat *viewNear, GLfloat *viewFar);

extern void cmapSetCubeMapTexDim(CubeMap *cmap, GLsizei texdim);
extern void cmapSetViewTexDim(CubeMap *cmap, GLsizei texdim);
extern void cmapGetCubeMapTexDim(CubeMap *cmap, GLsizei *texdim);
extern void cmapGetViewTexDim(CubeMap *cmap, GLsizei *texdim);

extern void cmapSetContextData(CubeMap *cmap, void *context);
extern void cmapGetContextData(CubeMap *cmap, void **context);

extern void cmapSetPositionLightsFunc(CubeMap *cmap, void (*positionLights)(int view, void *context));
extern void cmapSetDrawViewFunc(CubeMap *cmap, void (*drawView)(int view, void *context));
extern void cmapGetPositionLightsFunc(CubeMap *cmap, void (**positionLights)(int view, void *context));
extern void cmapGetDrawViewFunc(CubeMap *cmap, void (**drawView)(int view, void *context));

extern void cmapGenViewTex(CubeMap *cmap, int view);
extern void cmapGenViewTexs(CubeMap *cmap);
extern void cmapGenCubeMapFromViewTexs(CubeMap *cmap);
extern void cmapGenCubeMap(CubeMap *cmap);
extern void cmapGenCubeMapWithOneViewTex(CubeMap *cmap);

extern int cmapRvecToSt(float rvec[3], float st[2]);
extern void cmapStToRvec(float *st, float *rvec);

extern void cmapRenderCubeMappedObj(CubeMap *cmap);
extern void cmapNoRenderCubeMappedObj(void);

#ifdef __cplusplus
}
#endif
#endif /* __glcmap_h__ */
