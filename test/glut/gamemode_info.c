
/* Copyright (c) Mark J. Kilgard, 2000. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* gamemode_info.c - enumerate all the possible game mode configurations */

#include <stdio.h>
#include <GL/glut.h>

int
main(int argc, char **argv)
{
  char buffer[300];
  int active, possible, width, height, pixelDepth, hertz;
  int n;

  glutInit(&argc, argv);
  active = glutGameModeGet(GLUT_GAME_MODE_ACTIVE);
  if (active) {
    fprintf(stderr,
      "gamemode_info: GLUT_GAME_MODE_ACTIVE should initially be false!\n");
    exit(1);
  }
  for (n=1, possible=1; possible; n++) {
    sprintf(buffer, "num=%d", n);
    glutGameModeString(buffer);
    possible = glutGameModeGet(GLUT_GAME_MODE_POSSIBLE);
    if (possible) {
      width = glutGameModeGet(GLUT_GAME_MODE_WIDTH);
      height = glutGameModeGet(GLUT_GAME_MODE_HEIGHT);
      pixelDepth = glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH);
      hertz = glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE);
      printf(" %3d: %dx%d:%d@%d\n", n, width, height, pixelDepth, hertz);
    }
  }
  if (n == 1) {
    printf("No possible game mode configurations.\n");
  }
  return 0;             /* ANSI C requires main to return int. */
}
