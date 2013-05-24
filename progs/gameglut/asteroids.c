
/* Copyright (c) Mark J. Kilgard, 1997, 1998. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* modificatons Copyright (c) Ralph Giles, 1999 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "asteroids.h"

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* random float between 0 and x */
#define frand(x)        ((x)*(float)rand()/RAND_MAX)

#define MAX_FRAMERATE 60
#define MAX_SLEEPTIME 10000 /* don't sleep for more than 10 msec */

#define MAX_X   40.0  /* screen coordinate dimensions */
#define MAX_Y   40.0

#define MAX_BULLETS 10
#define MAX_ROCKS 50

char *initialGameModeString = NULL;

float angle = 0.0;
int left, right;
int leftTime, rightTime;
int thrust, thrustTime;
int joyThrust = 0, joyLeft = 0, joyRight = 0;
float x = (MAX_X/2.0), y = (MAX_Y/2.0), xv, yv, v;
int shield = 0, joyShield = 0, cursor = 1;
int lastTime;
int paused = 0;
int resuming = 1;
int sleeptime = 1;
int originalWindow = 0, currentWindow;

Bullet bullet[MAX_BULLETS];
Rock rock[MAX_ROCKS];

/* Bullets routines */
int
allocBullet(void)
{
  int i;

  for (i=0; i<MAX_BULLETS; i++) {
    if (!bullet[i].inuse) {
      return i;
    }
  }
  return -1;
}

void
initBullet(int i, int time)
{
  float c = cos(angle*M_PI/180.0);
  float s = sin(angle*M_PI/180.0);

  bullet[i].inuse = 1;
  bullet[i].x = x + 2 * c;
  bullet[i].y = y + 2 * s;
  bullet[i].v = 0.025;
  bullet[i].xv = xv + c * bullet[i].v;
  bullet[i].yv = yv + s * bullet[i].v;
  bullet[i].expire = time + 1000;
}

void
advanceBullets(int delta, int time)
{
  int i;

  for (i=0; i<MAX_BULLETS; i++) {
    if (bullet[i].inuse) {
      float x, y;

      if (time > bullet[i].expire) {
        bullet[i].inuse = 0;
        continue;
      }
      x = bullet[i].x + bullet[i].xv * delta;
      y = bullet[i].y + bullet[i].yv * delta;
      x = x / MAX_X;
      bullet[i].x = (x - floor(x))*MAX_X;
      y = y / MAX_Y;
      bullet[i].y = (y - floor(y))*MAX_Y;
    }
  }
}

void
collideBullets(void)
{
  int i,j,k1,k2;
  float dx,dy;
  float x,y,r;
  float xv,yv,e;

  for (i=0; i<MAX_BULLETS; i++)
    if (bullet[i].inuse)
      for (j=0; j<MAX_ROCKS; j++)
        if (rock[j].inuse) {
          x = rock[j].x;
          y = rock[j].y;
          r = rock[j].radius;
          dx = abs(bullet[i].x - x);
          dy = abs(bullet[i].y - y);
          if (dx + dy < r) {    /* quick manhattan metric */
            xv = rock[j].xv;
            yv = rock[j].yv;
            /* remove bullet and rock from circulation */
            rock[j].inuse = bullet[i].inuse = 0;
            if (r > 1.2) {
              /* split larger asteroids into two smaller ones */
              k1 = allocRock();
              if (k1 >= 0) {
                initRock(k1, r/2.0 + frand(0.5) - 0.3);
                rock[k1].x = x + frand(r) - r/2.0;
                rock[k1].y = y + frand(r) - r/2.0;
              }  
              k2 = allocRock();
              if (k2 >= 0) {
                initRock(k2, r/2.0 + frand(0.5) - 0.2);
                rock[k2].x = x + frand(r) - r/2.0;
                rock[k2].y = y + frand(r) - r/2.0;
              }
              if (k1 + k2 >= 0) {
                /* conserve momentum assuming equal masses */
                e = rock[k1].xv + rock[k2].xv - xv;
                rock[k1].xv -= e/2.0;
                rock[k2].xv -= e/2.0;
                e = rock[k1].yv + rock[k2].yv - yv;
                rock[k1].yv -= e/2.0;
                rock[k2].yv -= e/2.0;
              }
            }
            j = MAX_ROCKS;
          }
        }
}

void
shotBullet(void)
{
  int entry;

  entry = allocBullet();
  if (entry >= 0) {
    initBullet(entry, glutGet(GLUT_ELAPSED_TIME));
  }
}

void
drawBullets(void)
{
  int i;

  glBegin(GL_POINTS);
  glColor3f(1.0, 0.0, 1.0);
  for (i=0; i<MAX_BULLETS; i++) {
    if (bullet[i].inuse) {
      glVertex2f(bullet[i].x, bullet[i].y);
    }
  }
  glEnd();
}

/* Rocks aka Asteroids */

int
allocRock(void)
{
  int i;

  for (i=0; i<MAX_ROCKS; i++) {
    if (!rock[i].inuse) {
      return i;
    }
  }
  return -1;
}

void
initRock(int i, float radius)
{
  float speed = frand(0.01);
  float dir = frand(2*M_PI); 
  
  if (i < 0 || i >= MAX_ROCKS) return;

  rock[i].inuse = 1;
  rock[i].x = frand(MAX_X);
  rock[i].y = frand(MAX_Y);
  rock[i].v = speed;
  rock[i].xv = speed * cos(dir);
  rock[i].yv = speed * sin(dir);
  rock[i].radius = radius;
  rock[i].spin = frand(360.0);
  rock[i].spinv = 1.3 * (frand(1.0) - 0.5);
}

void
initRocks(void)
{
  int i;

  for (i=0; i<4; i++) {
        initRock(i, 1.0 + frand(3.0));
  }
}

void
advanceRocks(int delta)
{
  int i;
  float x, y;
  
  for (i=0; i<MAX_ROCKS; i++) {
    if (rock[i].inuse) {
      x = rock[i].x + rock[i].xv * delta;
      y = rock[i].y + rock[i].yv * delta;
      x = x / MAX_X;
      rock[i].x = (x - floor(x))*MAX_X;
      y = y / MAX_Y;
      rock[i].y = (y - floor(y))*MAX_Y;
      rock[i].spin += rock[i].spinv;
    }
  }
}

void
drawRocks(void)
{
  int i;
  float r,theta;
  
  for (i=0; i<MAX_ROCKS; i++) {
    if (rock[i].inuse) {
      r = rock[i].radius;
      glPushMatrix();
      glTranslatef(rock[i].x, rock[i].y, 0.0);
      glRotatef(rock[i].spin, 0.0, 0.0, 1.0);
      glColor3f(1.0, 1.0, 0.0);
      glBegin(GL_LINE_LOOP);
      for (theta=0.0; theta<2.0*M_PI; theta += 0.76) {
        glVertex2f(1.2*r*cos(theta), r*sin(theta));
      }
      glEnd();
      glPopMatrix();
    }
  }
}


/* our noble Ship */

void
drawShip(float angle)
{
  float rad;

  glPushMatrix();
  glTranslatef(x, y, 0.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  if (thrust) {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.75, -0.5);
    glVertex2f(-1.75, 0);
    glVertex2f(-0.75, 0.5);
    glEnd();
  }
  glColor3f(1.0, 1.0, 0.4);
  glBegin(GL_LINE_LOOP);
  glVertex2f(2.0, 0.0);
  glVertex2f(-1.0, -1.0);
  glVertex2f(-0.5, 0.0);
  glVertex2f(-1.0, 1.0);
  glVertex2f(2.0, 0.0);
  glEnd();
  if (shield) {
    glColor3f(0.2, 0.1, 0.8);
    glBegin(GL_LINE_LOOP);
    for (rad=0.0; rad<12.0; rad += 1.0) {
      glVertex2f(2.3 * cos(2*rad/M_PI)+0.2, 2.0 * sin(2*rad/M_PI));
    }
    glEnd();
  }
  glPopMatrix();
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  drawShip(angle);
  drawRocks();
  drawBullets();
  glutSwapBuffers();
}

void
idle(void)
{
  int time, delta;

  time = glutGet(GLUT_ELAPSED_TIME);
  if (resuming) {
    lastTime = time;
    resuming = 0;
  }
  if (left) {
    delta = time - leftTime;
    angle = angle + delta * 0.4;
    leftTime = time;
  }
  if (right) {
    delta = time - rightTime;
    angle = angle - delta * 0.4;
    rightTime = time;
  }
  if (thrust) {
    delta = time - thrustTime;
    v = delta * 0.00004;
    xv = xv + cos(angle*M_PI/180.0) * v;
    yv = yv + sin(angle*M_PI/180.0) * v;
    thrustTime = time;
  }
  delta = time - lastTime;
  x = x + xv * delta;
  y = y + yv * delta;
  x = x / MAX_X;
  x = (x - floor(x))*MAX_X;
  y = y / MAX_Y;
  y = (y - floor(y))*MAX_Y;
  lastTime = time;
  advanceRocks(delta);
  advanceBullets(delta, time);
  collideBullets();
  glutPostWindowRedisplay(currentWindow);

  /* let's not waste time running at eccessive framerates */
  if (delta > 1.0/(float)MAX_FRAMERATE) {
    sleeptime -= 1;
    if (sleeptime < 1) sleeptime = 0;
  } else {
    sleeptime += 1;
    if (sleeptime > MAX_SLEEPTIME) sleeptime = MAX_SLEEPTIME;
  }
#if 0
  usleep(sleeptime);
#endif
}

void
visible(int vis)
{
  if (vis == GLUT_VISIBLE) {
    if (!paused) {
      glutIdleFunc(idle);
    }
  } else {
    glutIdleFunc(NULL);
  }
}

/* ARGSUSED1 */
void
key(unsigned char key, int px, int py)
{
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'A':
  case 'a':
    thrust = 1;
    thrustTime = glutGet(GLUT_ELAPSED_TIME);
    break;
  case 'S':
  case 's':
    shield = 1;
    break;
  case 'C':
  case 'c':
    cursor = !cursor;
    glutSetCursor(
      cursor ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
    break;
  case 'z':
  case 'Z':
    x = 20;
    y = 20;
    xv = 0;
    yv = 0;
    initRocks();
    break;
  case 'd':
    glutGameModeString("1280x1024:32@60");
    glutEnterGameMode();
    initWindow();
    break;
  case 'f':
    glutGameModeString("640x480:16@60");
    glutEnterGameMode();
    initWindow();
    break;
  case 'g':
    glutGameModeString("800x600:16@60");
    glutEnterGameMode();
    initWindow();
    break;
  case 'h':
    if (initialGameModeString) {
      glutGameModeString(initialGameModeString);
      glutEnterGameMode();
      initWindow();
    }
    break;
  case 'l':
    if (originalWindow != 0 && currentWindow != originalWindow) {
      glutLeaveGameMode();
      currentWindow = originalWindow;
    }
    break;
  case 'P':
  case 'p':
    paused = !paused;
    if (paused) {
      glutIdleFunc(NULL);
    } else {
      glutIdleFunc(idle);
      resuming = 1;
    }
    break;
  case 'Q':
  case 'q':
  case ' ':
    shotBullet();
    break;
  case 'i':
    printf("%dx%d+%d+%d of depth %d of format %d\n",
      glutGet(GLUT_WINDOW_WIDTH),
      glutGet(GLUT_WINDOW_HEIGHT),
      glutGet(GLUT_WINDOW_X),
      glutGet(GLUT_WINDOW_Y),
      glutGet(GLUT_WINDOW_BUFFER_SIZE),
      glutGet(GLUT_WINDOW_FORMAT_ID));
    break;
  }
}

/* ARGSUSED1 */
void
keyup(unsigned char key, int x, int y)
{
  switch (key) {
  case 'A':
  case 'a':
    thrust = 0;
    break;
  case 'S':
  case 's':
    shield = 0;
    break;
  }
}

/* ARGSUSED1 */
void
special(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_F1:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    break;
  case GLUT_KEY_F2:
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    break;
  case GLUT_KEY_UP:
    thrust = 1;
    thrustTime = glutGet(GLUT_ELAPSED_TIME);
    break;
  case GLUT_KEY_LEFT:
    left = 1;
    leftTime = glutGet(GLUT_ELAPSED_TIME);
    break;
  case GLUT_KEY_RIGHT:
    right = 1;
    rightTime = glutGet(GLUT_ELAPSED_TIME);
    break;
  }
}

/* ARGSUSED1 */
void
specialup(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    thrust = 0;
    break;
  case GLUT_KEY_LEFT:
    left = 0;
    break;
  case GLUT_KEY_RIGHT:
    right = 0;
    break;
  }
}

/* ARGSUSED3 */
void
joystick(unsigned int buttons, int x, int y, int z)
{
  if (buttons & 0x1) {
    thrust = 1;
    thrustTime = glutGet(GLUT_ELAPSED_TIME);
    joyThrust = 1;
  } else {
    if (joyThrust) {
      thrust = 0;
      joyThrust = 0;
    }
  }
  if (buttons & 0x2) {
    shotBullet();
  }
  if (buttons & 0x4) {
    shield = 1;
    joyShield = 1;
  } else {
    if (joyShield) {
      shield = 0;
      joyShield = 0;
    }
  }
  if (x < -300) {
    left = 1;
    leftTime = glutGet(GLUT_ELAPSED_TIME);
    joyLeft = 1;
  } else {
    /* joyLeft helps avoid "joystick in neutral"
       from continually stopping rotation. */
    if (joyLeft) {
      left = 0;
      joyLeft = 0;
    }
  }
  if (x > 300) {
    right = 1;
    rightTime = glutGet(GLUT_ELAPSED_TIME);
    joyRight = 1;
  } else {
    /* joyRight helps avoid "joystick in neutral"
       from continually stopping rotation. */
    if (joyRight) {
      right = 0;
      joyRight = 0;
    }
  }
}

void
initWindow(void)
{
  glutIgnoreKeyRepeat(1);

  glutDisplayFunc(display);
  glutVisibilityFunc(visible);
  glutKeyboardFunc(key);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(special);
  glutSpecialUpFunc(specialup);
  glutJoystickFunc(joystick, 100);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, MAX_X, 0, MAX_Y, 0, MAX_Y);
  glMatrixMode(GL_MODELVIEW); 
  glPointSize(3.0);

  currentWindow = glutGetWindow();
}

int
main(int argc, char **argv)
{
  int i;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  for (i=1; i<argc; i++) {
    if (!strcmp(argv[i], "-fullscreen")) {
      if (!initialGameModeString) {
        initialGameModeString = "640x480:16@60";
      }
    } else
    if (!strcmp(argv[i], "-custom")) {
      i++;
      if (i >= argc) {
        printf("asteroids: -custom option requires a game mode string\n");
        exit(1);
      }
      initialGameModeString = argv[i];
    }
  }

  if (initialGameModeString) {
    glutGameModeString(initialGameModeString);
    glutEnterGameMode();
  } else {
    originalWindow = glutCreateWindow("asteroids");
  }

  /* antialiasing */
/*
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth (1.5);
*/

  initWindow();

  initRocks();

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
