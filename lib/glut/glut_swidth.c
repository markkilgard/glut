
/* Copyright (c) Mark J. Kilgard, 1995, 2001. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include "glutint.h"
#include "glutstroke.h"

/* glutStrokeWidth and glutStrokeLength had bugs in their implementation
   prior to GLUT 3.8.  While the width of a stroke font character is a
   float, prior to GLUT 3.8, glutStrokeWidth returns the width truncated
   to an integer.  Additionally, prior to GLUT 3.8, glutStrokeLength
   accumulated the length of a string with integer truncated widths
   meaning that glutStrokeLength underestimated the actual stroke font
   string length.

   GLUT 3.8 fixes glutStrokeLength to accumulate its string length
   with a float.  GLUT 3.8 also adds the routines glutStrokeWidthf and
   glutStrokeLengthf that return accurate float widths and lengths.

   Use of glutStrokeWidth and glutStrokeLength is deprecated in favor
   of using glutStrokeWidthf and glutStrokeLengthf respectively. */

/* CENTRY */
int GLUTAPIENTRY 
glutStrokeWidth(GLUTstrokeFont font, int c)
{
  StrokeFontPtr fontinfo;
  const StrokeCharRec *ch;

#if defined(_WIN32)
  fontinfo = (StrokeFontPtr) __glutFont(font);
#else
  fontinfo = (StrokeFontPtr) font;
#endif

  if (c < 0 || c >= fontinfo->num_chars) {
    return 0;
  }
  ch = &(fontinfo->ch[c]);
  if (ch) {
    return ch->right;
  } else {
    return 0;
  }
}

float GLUTAPIENTRY 
glutStrokeWidthf(GLUTstrokeFont font, int c)
{
  StrokeFontPtr fontinfo;
  const StrokeCharRec *ch;

#if defined(_WIN32)
  fontinfo = (StrokeFontPtr) __glutFont(font);
#else
  fontinfo = (StrokeFontPtr) font;
#endif

  if (c < 0 || c >= fontinfo->num_chars) {
    return 0;
  }
  ch = &(fontinfo->ch[c]);
  if (ch) {
    return ch->right;
  } else {
    return 0;
  }
}

int GLUTAPIENTRY 
glutStrokeLength(GLUTstrokeFont font, const unsigned char *string)
{
  StrokeFontPtr fontinfo;
  float length = 0.0;
  const StrokeCharRec *ch;

#if defined(_WIN32)
  fontinfo = (StrokeFontPtr) __glutFont(font);
#else
  fontinfo = (StrokeFontPtr) font;
#endif

  for (; *string != '\0'; string++) {
    unsigned char c;

    c = *string;
    if (c < fontinfo->num_chars) {
      ch = &(fontinfo->ch[c]);
      if (ch) {
        length += ch->right;
      }
    }
  }
  /* Truncate to an int to conform to glutStrokeLength's unfortunate
     return type. */
  return (int) length;
}

float GLUTAPIENTRY 
glutStrokeLengthf(GLUTstrokeFont font, const unsigned char *string)
{
  StrokeFontPtr fontinfo;
  float length = 0.0;
  const StrokeCharRec *ch;

#if defined(_WIN32)
  fontinfo = (StrokeFontPtr) __glutFont(font);
#else
  fontinfo = (StrokeFontPtr) font;
#endif

  for (; *string != '\0'; string++) {
    unsigned char c;

    c = *string;
    if (c < fontinfo->num_chars) {
      ch = &(fontinfo->ch[c]);
      if (ch) {
        length += ch->right;
      }
    }
  }
  return length;
}

/* ENDCENTRY */
