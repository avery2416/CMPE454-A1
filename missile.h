// Missile class


#ifndef MISSILE_H
#define MISSILE_H

#include "linalg.h"
#include "gpuProgram.h"
#include "drawSegs.h"

#define GRAVITY 9.8


class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vec3 initPosition, vec3 initVelocity, float stopAtY, vec3 _colour ) {
    pos0 = initPosition;
    pos1 = initPosition;
    velocity = initVelocity;
    colour = _colour;
    destY = stopAtY;
  }

  // Draw the missile and its trail

void draw(GPUProgram *gpu, mat4 &worldToScreen) {
    // Define the missile tube vertices in local space
    const int numPts = 6; // Example: Tube with 6 points around its circumference
    vec3 points[numPts] = {
        vec3(0, 0.1, -0.5), // Tail
        vec3(0.1, 0, -0.5),
        vec3(0, -0.1, -0.5),
        vec3(-0.1, 0, -0.5),
        vec3(0, 0.1, 0.5),  // Tip
        vec3(0, -0.1, 0.5)
    };

    vec3 colours[numPts] = { colour, colour, colour, colour, colour, colour };

    // Compute the orientation of the missile
    vec3 dir = (pos1 - pos0).normalize(); // Normalize direction of flight
    vec3 up(0, 1, 0);                     // Up vector (arbitrary reference)
    vec3 right = up ^ dir;                // Perpendicular vector
    right = right.normalize();            // Normalize the right vector
    up = dir ^ right;                     // Recalculate up to ensure orthogonality

    // Build the rotation matrix
    mat4 rotation;
    rotation[0] = vec4(right.x, right.y, right.z, 0); // Right vector
    rotation[1] = vec4(up.x, up.y, up.z, 0);          // Up vector
    rotation[2] = vec4(dir.x, dir.y, dir.z, 0);       // Direction vector
    rotation[3] = vec4(0, 0, 0, 1);                  // Homogeneous coordinate

    // Combine world-to-screen transformation with position and orientation
    mat4 translation;
    translation[0] = vec4(1, 0, 0, 0);
    translation[1] = vec4(0, 1, 0, 0);
    translation[2] = vec4(0, 0, 1, 0);
    translation[3] = vec4(pos1.x, pos1.y, pos1.z, 1);

    mat4 M = worldToScreen * translation * rotation;

    drawSegs(gpu, GL_LINE_LOOP, points, colours, numPts, M);
  }



  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;

    // YOUR CODE HERE (Step 6)
    velocity.y -= GRAVITY * deltaT;
  }

  // Return the current position 

  vec3 position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {

    // YOUR CODE HERE (Step 3)

    return false;
  }

 private:

  vec3 pos0;       // start position
  vec3 pos1;       // current position
  vec3 velocity;   // velocity
  vec3 colour;     // colour of missile trail
  float destY;     // y position at destination
};


#endif
