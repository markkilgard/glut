
/* Copyright (c) Mark J. Kilgard, 1999. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* This test is supposed to measure the GLUT implementation's
   glutGet(GLUT_ELAPSED_TIME) resolution. */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define RUNS 5
#define LOOPS 10000

int
main(int argc, char **argv)
{
  int start, end = 0;
  double interval;
  double period;
  double minInterval = 1000000, maxInterval = 0;
  double minPeriod = 1000000, maxPeriod = 0;
  double totalPeriod = 0.0;
  double totalInterval = 0.0;
  int minGap = 100000, maxGap = 0;
  double totalGaps = 0.0;
  double avgResolution;
  int i, j;

  glutInit(&argc, argv);
  for (i=0; i<RUNS; i++) {
    printf("run %d\n", i);
    start = glutGet(GLUT_ELAPSED_TIME);
    for (j=0; j<LOOPS; j++) {
      end = glutGet(GLUT_ELAPSED_TIME);
    }
    period = end - start;
    if (period < minPeriod) {
      minPeriod = period;
    }
    if (period > maxPeriod) {
      maxPeriod = period;
    }
    totalPeriod += period/LOOPS;
  }
  printf("max period = %f milliseconds\n", maxPeriod/LOOPS);
  printf("min period = %f milliseconds\n", minPeriod/LOOPS);
  printf("avg period = %f milliseconds\n", totalPeriod/RUNS);

  start = glutGet(GLUT_ELAPSED_TIME);
  do {
    end = glutGet(GLUT_ELAPSED_TIME);
  } while (end == start);

  for (i=0; i<RUNS; i++) {
    j = 0;
    start = glutGet(GLUT_ELAPSED_TIME);
    do {
      end = glutGet(GLUT_ELAPSED_TIME);
      j++;
    } while (end == start);
    interval = end - start;
    if (j < minGap) {
      minGap = j;
    }
    if (j > maxGap) {
      maxGap = j;
    }
    if (interval < minInterval) {
      minInterval = interval;
    }
    if (interval > maxInterval) {
      maxInterval = interval;
    }
    totalGaps += j;
  }

  printf("max gap = %d calls\n", maxGap);
  printf("min gap = %d calls\n", minGap);
  printf("avg gap = %f calls\n", totalGaps/RUNS);

  printf("max interval = %f\n", maxInterval);
  printf("min interval = %f\n", minInterval);

  avgResolution = (totalGaps/RUNS) * (totalPeriod/RUNS);
  printf("avg resolution = %f milliseconds\n", avgResolution);

#if _WIN32
  start = GetTickCount();
  do {
    end = GetTickCount();
  } while (end == start);
  printf("%d %d %d\n", start, end, end-start);
  start = GetTickCount();
  do {
    end = GetTickCount();
  } while (end == start);
  printf("%d %d %d\n", start, end, end-start);
  start = GetTickCount();
  do {
    end = GetTickCount();
  } while (end == start);
  printf("%d %d %d\n", start, end, end-start);
#endif

  return 0;             /* ANSI C requires main to return int. */
}

