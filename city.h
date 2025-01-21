// A city


#ifndef CITY_H
#define CITY_H


#include "headers.h"
#include "drawSegs.h"


#define CITY_WIDTH 0.04
#define CITY_HEIGHT 0.03

class City {
 public:

  // Constructors 

  City() {}

  City( vec3 _pos ) {
    alive = true;
    pos = _pos;
  }

  // Draw the city

  void draw( GPUProgram *gpu, mat4 &worldToScreen ) {

    // YOUR CODE HERE (Step 4)
    const int numPts = 4;

    vec3 points[numPts] = {
        vec3(CITY_WIDTH, 0, 0),
        vec3(CITY_WIDTH, CITY_HEIGHT, 0),
        vec3(-CITY_WIDTH, CITY_HEIGHT, 0),
        vec3(-CITY_WIDTH, 0, 0) // Static rectangle
    };

    vec3 colours[numPts] = {
        vec3(1, 1, 1),
        vec3(1, 1, 1),
        vec3(1, 1, 1),
        vec3(1, 1, 1)
    };

    // Transformation matrix for the city's position
    mat4 M = worldToScreen * translate(pos.x, pos.y, 0);
    
    drawSegs(gpu, GL_LINE_LOOP, points, colours, numPts, M);
  }

  void deactivate() {
    alive = false;
  }

  bool isAlive() {
    return alive;
  }

  vec3 position() {
    return pos;
  }

 private:
  
  bool alive;
  vec3 pos;
};
  


#endif
