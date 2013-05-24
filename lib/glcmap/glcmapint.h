#ifndef __glcmapint_h__
#define __glcmapint_h__

/* Copyright (c) Mark J. Kilgard, 1998. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include <GL/glcmap.h>

/* EXT_texture_cube_map defines from <GL/gl.h> */
#define GL_NORMAL_MAP                       0x8511
#define GL_REFLECTION_MAP                   0x8512
#define GL_TEXTURE_CUBE_MAP                 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP         0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X      0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X      0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y      0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y      0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z      0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z      0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP           0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE        0x851C

enum { X = 0, Y = 1, Z = 2 };

#if 0
#define INITFACE(mesh) \
    int steps = mesh->steps; \
    int sqsteps = mesh->steps * mesh->steps

#define FACE(side,y,x) \
    mesh->face[(side)*sqsteps + (y)*steps + (x)]

#define FACExy(side,i,j) \
    (&FACE(side,i,j).x)

#define FACEst(side,i,j) \
    (&FACE(side,i,j).s)

#define INITBACK(mesh) \
    int allrings = mesh->rings + mesh->edgeExtend; \
    int ringedspokes = allrings * mesh->steps

#define BACK(edge,ring,spoke) \
    mesh->back[(edge)*ringedspokes + (ring)*mesh->steps + (spoke)]

#define BACKxy(edge,ring,spoke) \
    (&BACK(edge,ring,spoke).x)

#define BACKst(edge,ring,spoke) \
    (&BACK(edge,ring,spoke).s)
#endif

typedef struct _STXY {
    GLfloat s, t;
    GLfloat x, y;
} STXY;

typedef struct _CubeMapMesh {

    int refcnt;

    int steps;
    int rings;
    int edgeExtend;

    STXY *face;
    STXY *back;

} CubeMapMesh;

struct _CubeMap {

    /* Texture object id. */
    GLuint cmapTexObj;

    /* Flags */
    CubeMapFlags flags;

    /* Texture dimensions must be a power of two. */
    int viewTexDim;  /* view texture dimension */
    int cmapTexDim;  /* sphere map texture dimension */

    /* Viewport origins for view and sphere map rendering. */
    int viewOrigin[2];
    int cmapOrigin[2];

    /* Viewing vectors. */
    GLfloat eye[3];
    GLfloat up[3];
    GLfloat obj[3];

    /* Projection parameters. */
    GLfloat viewNear;
    GLfloat viewFar;

    /* Rendering callbacks. */
    void (*positionLights)(int view, void *context);
    void (*drawView)(int view, void *context);

    /* Application specified callback data. */
    void *context;

};

#endif /* __glcmapint_h__ */
