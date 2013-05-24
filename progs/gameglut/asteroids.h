
/* Copyright (c) Mark J. Kilgard, 1997, 1998. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* modificatons Copyright (c) Ralph Giles, 1999 */

/* structures */

typedef struct {
  int inuse;

  float x;
  float y;

  float v;
  float xv;
  float yv;

  int expire;
} Bullet;

typedef struct {
  int inuse;

  float x;
  float y;

  float v;
  float xv;
  float yv;

  float radius;

  float spin;
  float spinv;
} Rock;


/* prototypes */

/* Bullet routines */
int  allocBullet(void);
void initBullet(int i, int time);
void advanceBullets(int delta, int time);
void collideBullets(void);
void shotBullet(void);
void drawBullets(void);

/* Rocks aka Asteroids */
int  allocRock(void);
void initRock(int i, float radius);
void initRocks(void);
void advanceRocks(int delta);
void drawRocks(void);

/* our noble Ship */
void drawShip(float angle);

void initWindow(void);

void display(void);
void idle(void);
void visible(int vis);

/* ARGSUSED1 */
void key(unsigned char key, int px, int py);
void keyup(unsigned char key, int x, int y);
void special(int key, int x, int y);
void specialup(int key, int x, int y);

/* ARGSUSED3 */
void joystick(unsigned int buttons, int x, int y, int z);


/* end */
