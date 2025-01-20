// fgStroke.h
//
// from freeglut


#ifndef FGSTROKE
#define FGSTROKE

#include "headers.h"


// The stroke font structures
//
// From freeglut

typedef struct tagSFG_StrokeVertex SFG_StrokeVertex;
struct tagSFG_StrokeVertex
{
  GLfloat         X, Y;
};

typedef struct tagSFG_StrokeStrip SFG_StrokeStrip;
struct tagSFG_StrokeStrip
{
  int             Number;
  const SFG_StrokeVertex* Vertices;
};

typedef struct tagSFG_StrokeChar SFG_StrokeChar;
struct tagSFG_StrokeChar
{
  GLfloat         Right;
  int             Number;
  const SFG_StrokeStrip* Strips;
};

typedef struct tagSFG_StrokeFont SFG_StrokeFont;
struct tagSFG_StrokeFont
{
  char*           Name;                       /* The source font name      */
  int             Quantity;                   /* Number of chars in font   */
  GLfloat         Height;                     /* Height of the characters  */
  const SFG_StrokeChar** Characters;          /* The characters mapping    */
};


extern SFG_StrokeFont fgStrokeMonoRoman;

#endif
