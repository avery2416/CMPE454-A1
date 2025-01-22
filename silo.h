// A Silo


#ifndef SILO_H
#define SILO_H

#include "headers.h"
#include "drawSegs.h"
#include "strokefont.h"

#define SILO_RADIUS 0.04
#define NUM_SEMICIRCLE_PTS 100


class Silo {

 public:

  // Constructors 

  Silo() {}

  Silo( vec3 _pos ) {

    roundsLeft = 15;
    alive = true;
    pos = _pos;

    strokeFont = new StrokeFont();
  }

  bool canShoot() {
    return (alive && roundsLeft > 0);
  }

  void decrMissiles() {
    roundsLeft--;
  }

  vec3 position() {
    return pos;
  }

  int missileCount() {
    return roundsLeft;
  }

  bool isAlive() {
    return alive;
  }

  void deactivate() {
    alive = false;
  }

  // Draw the silo

  void draw( GPUProgram *gpu, mat4 &worldToScreen ) {

    // YOUR CODE HERE (Step 4)
    
    const vec3 colour(1, 1, 1);

    vec3 points[NUM_SEMICIRCLE_PTS];
    vec3 colours[NUM_SEMICIRCLE_PTS];

    // Define points in object-local coordinates (centered at origin)
    for (int i = 0; i < NUM_SEMICIRCLE_PTS; i++) {
        float theta = i * M_PI / (float) NUM_SEMICIRCLE_PTS;
        points[i] = vec3(SILO_RADIUS * cos(theta), SILO_RADIUS * sin(theta), 0);
        colours[i] = colour;
    }

    // Build the transformation matrix
    mat4 M = worldToScreen * translate(pos);

    drawSegs(gpu, GL_LINE_LOOP, points, colours, NUM_SEMICIRCLE_PTS, M);

    static char buffer[2];

    float xRemMissiles = M[0][3];
    float yRemMissiles = M[1][3] + 0.025;

    sprintf( buffer, "%d", roundsLeft);
    strokeFont->drawStrokeString( buffer, xRemMissiles, yRemMissiles, 0.07, 0, CENTRE, vec3(.5, 1, 1) );
  }


 private:

  int roundsLeft;
  bool alive;
  vec3 pos;

  StrokeFont *strokeFont;  
};
  

#endif
