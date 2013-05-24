
/* Copyright (c) Mark J. Kilgard, 1998.  */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* cmap_buildcmap.c - automatically builds sphere map */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <GL/glcmap.h>
#include <GL/glu.h>

#include "glcmapint.h"

#if defined(GL_EXT_texture_object) && !defined(GL_VERSION_1_1)
#define glBindTexture(A,B)     glBindTextureEXT(A,B)
#endif
#if defined(GL_EXT_copy_texture) && !defined(GL_VERSION_1_1)
#define glCopyTexImage2D(A, B, C, D, E, F, G, H)    glCopyTexImage2DEXT(A, B, C, D, E, F, G, H)
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE  0x812F  /* Clamp to edge texture wrap mode. */
#endif

static int
supportsOneDotTwo(void)
{
  const char *version;
  int major, minor;

  version = (char *) glGetString(GL_VERSION);
  if (sscanf(version, "%d.%d", &major, &minor) == 2) {
    return major > 1 || minor >= 2;
  }
  return 0;            /* OpenGL version string malformed! */
}

/* Based on glutExtensionSupported. */
static int 
supportsExtension(const char *extension)
{
  static const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;

  if (!extensions) {
    extensions = glGetString(GL_EXTENSIONS);
  }
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string.  Don't be fooled by sub-strings,
     etc. */
  start = extensions;
  for (;;) {
    /* If your application crashes in the strstr routine below,
       you are probably calling glutExtensionSupported without
       having a current window.  Calling glGetString without
       a current OpenGL context has unpredictable results.
       Please fix your program. */
    where = (GLubyte *) strstr((const char *) start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ') {
      if (*terminator == ' ' || *terminator == '\0') {
        return 1;
      }
    }
    start = terminator;
  }
  return 0;
}

/* Warning:  This routine only tracks the clamp to edge support based on
   the first context it is invoked with.  If you use multiple contexts
   with different OpenGL versions and extensions, this could cause you
   problems. */
static int
supportsClampToEdge(void)
{
  static int hasClampToEdge = -1;  /* Negative means yet undetermined. */

  if (hasClampToEdge < 0) {
    hasClampToEdge = supportsExtension("GL_EXT_texture_edge_clamp") || supportsOneDotTwo();
  }
  return hasClampToEdge;
}

static void
copyImageToTexture(CubeMap *cmap,
                   int view,
                   int origin[2], int texdim)
{
    GLuint faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X+view;
    static GLubyte pixels[256][256][3];  /* XXX fix me. */

    glBindTexture(GL_TEXTURE_CUBE_MAP, cmap->cmapTexObj);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);
    if (cmap->flags & CMAP_GENERATE_MIPMAPS) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    /* Clamp to avoid artifacts from wrap around in texture. */
    if (supportsClampToEdge()) {
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }

    if (cmap->flags & CMAP_GENERATE_MIPMAPS) {
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(origin[X], origin[Y], texdim, texdim,
            GL_RGB, GL_UNSIGNED_BYTE, pixels);
        gluBuild2DMipmaps(faceTarget, 3, texdim, texdim,
            GL_RGB, GL_UNSIGNED_BYTE, pixels);       
    } else {
        glCopyTexImage2D(faceTarget, 0, GL_RGB,
        origin[X], origin[Y], texdim, texdim, 0);
    }
}

static const struct {
    GLfloat angle;
    GLfloat x, y, z;
} faceInfo[6] = {
#if 0
    {   0.0, +1.0,  0.0,  0.0 },  /* front */
    {  90.0, -1.0,  0.0,  0.0 },  /* top */
    {  90.0, +1.0,  0.0,  0.0 },  /* bottom */
    {  90.0,  0.0, -1.0,  0.0 },  /* left */
    {  90.0,  0.0, +1.0,  0.0 },  /* right */
    { 180.0, -1.0,  0.0,  0.0 }   /* back */
#else
    {  90.0,  0.0, -1.0,  0.0 },  /* front */
    {  90.0,  0.0,  1.0,  0.0 },  /* front */

    {  90.0, -1.0,  0.0,  0.0 },  /* front */
    {  90.0,  1.0,  0.0,  0.0 },  /* front */

    {   0.0, +1.0,  0.0,  0.0 },  /* front */
    { 180.0, +1.0,  0.0,  0.0 },  /* front */
#endif
};

static void
configFace(CubeMap *cmap, int view)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (view != 5) {
      glRotatef(180, 0, 0, 1);
    }
    glRotatef(faceInfo[view].angle,
        faceInfo[view].x, faceInfo[view].y, faceInfo[view].z);
    gluLookAt(cmap->obj[X], cmap->obj[Y], cmap->obj[Z],  /* "eye" at object */
              cmap->eye[X], cmap->eye[Y], cmap->eye[Z],  /* looking at eye */
              cmap->up[X], cmap->up[Y], cmap->up[Z]);
}

static void
initGenViewTex(CubeMap *cmap)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.0, cmap->viewNear, cmap->viewFar);
    glViewport(cmap->viewOrigin[X], cmap->viewOrigin[Y],
        cmap->viewTexDim, cmap->viewTexDim);
    glScissor(0, 0, cmap->viewTexDim, cmap->viewTexDim);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
}

static void
genViewTex(CubeMap *cmap, int view)
{
    configFace(cmap, view);
        assert(cmap->positionLights);
    cmap->positionLights(view, cmap->context);
        assert(cmap->drawView);
    cmap->drawView(view, cmap->context);
}

void
cmapGenViewTex(CubeMap *cmap, int view)
{
    initGenViewTex(cmap);
    genViewTex(cmap, view);
    copyImageToTexture(cmap, view,
        cmap->viewOrigin, cmap->viewTexDim);
}

void
cmapGenViewTexs(CubeMap *cmap)
{
    int view;

    initGenViewTex(cmap);

    for (view=0; view<6; view++) {
        genViewTex(cmap, view);
        copyImageToTexture(cmap, view,
            cmap->viewOrigin, cmap->viewTexDim);
    }
}

void
cmapGenCubeMap(CubeMap *cmap)
{
    cmapGenViewTexs(cmap);
}

