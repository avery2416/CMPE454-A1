// world.cpp


#include "headers.h"
#include "world.h"
#include "mc.h"


#define BORDER  0.05                      // border on screen inside which world is drawn

#define INIT_OUTGOING_MISSILE_SPEED 0.7   // world units per second



// Draw each of the objects in the world

void World::draw() 

{
  gpu->activate();
    
  for (int i=0; i<silos.size(); i++)
    if (silos[i].isAlive())
      silos[i].draw( gpu, worldToScreen );

  for (int i=0; i<cities.size(); i++)
    if (cities[i].isAlive())
      cities[i].draw( gpu, worldToScreen );

  for (int i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw( gpu, worldToScreen );

  for (int i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw( gpu, worldToScreen );

  for (int i=0; i<explosions.size(); i++)
    explosions[i].draw( gpu, worldToScreen );

  gpu->deactivate();

  // Status message

  static char gameTitleBuffer[1000];

  sprintf( gameTitleBuffer, "MISSILE COMMAND" );
  strokeFont->drawStrokeString( gameTitleBuffer, 0, 0.85, 0.07, 0, CENTRE, vec3(0.2,0.7,0.5) );

  static char scoreTitleBuffer[1000];

  sprintf( scoreTitleBuffer, "Score" );
  strokeFont->drawStrokeString( scoreTitleBuffer, 0, 0.025, 0.05, 0, CENTRE, vec3(1,0.7,0.5) );

  static char scoreBuffer[1000];

  sprintf( scoreBuffer, "%d", score );
  strokeFont->drawStrokeString( scoreBuffer, 0, -0.025, 0.05, 0, CENTRE, vec3(1,0.7,0.5) );
}


// Process a keystroke
//
// Used to fire missiles


void World::handleKeystroke( int key, float x, float y )

{
  // YOUR CODE HERE (Step 1)
  float screenMin = -0.98f;
  float screenMax = 0.98f;

  float worldX = (x - screenMin) / (screenMax - screenMin);
  float worldY = ((y - screenMin) / (screenMax - screenMin)) * worldTop;
  
  vec3 worldMousePos = vec3(worldX, worldY, 0);

  // Handle key press
    
  switch (key) {

  case 'S':
    fireMissile( 0, worldMousePos );  // left silo
    break;

  case 'D':
    fireMissile( 1, worldMousePos );  // middle silo
    break;

  case 'F':
    fireMissile( 2, worldMousePos );  // right silo
    break;
  }
}


// Compute the worldTop and worldToScrren transform based on the
// window dimensions.  The world is [0,1] x [0,worldTop] and gets
// mapped to the screen coordinates [-1,1] x [-1,1] but with a small
// BORDER of 0.02, so the world *really* gets mapped to
// [-0.98,0.98]x[-0.98,0.98].

void World::reshape( int width, int height )

{
  const float windowLeft   = -1 + BORDER;
  const float windowRight  =  1 - BORDER;
  const float windowBottom = -1 + BORDER;
  const float windowTop    =  1 - BORDER;

  // The world is 1 unit wide, so the world's height must be the
  // window's aspect ratio (height/width).
  
  worldTop = height / (float) width;

  worldToScreen =
    translate( windowLeft, windowBottom, 0 ) * 
    scale( (windowRight - windowLeft) / (1 - 0), (windowTop - windowBottom) / (worldTop - 0), 1 );
}


// Update the state of the world after some time interval, deltaT


void World::updateState( float deltaT )

{
  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missle generation
  // increases with time.
  //
  // YOUR CODE HERE (Step 5a)
  float spawnProbability = 0.005f + (currentTime * 0.00001f);
  if (rand() / (float)RAND_MAX < spawnProbability) {
    float startX = rand() / (float)RAND_MAX;
    // float destX = rand() / (float)RAND_MAX;
    vec3 start(startX, worldTop, 0);
    vec3 velocity(rand() / (float)RAND_MAX*0.2 - 0.1, -0.1, 0);

    missilesIn.add(Missile(start, velocity, 0, vec3(1, 1, 1)));
  }  



  // Look for terminating missiles
  
  for (int i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
    
      // YOUR CODE HERE (Step 5b)
      explosions.add(Circle(missilesIn[i].position(), 0.07, 0.07, vec3(1, 0.5, 0.5), false));
      missilesIn.remove(i);
      i--;
    }

  for (int i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
    
      // YOUR CODE HERE (Step 5b)
      explosions.add(Circle(missilesOut[i].position(), 0.07, 0.04, vec3(0.5,0.5,1), true));
      missilesOut.remove(i);
      i--;
    }

  // Look for terminating explosions

  for (int i=0; i<explosions.size(); i++)
    if (explosions[i].radius() >= explosions[i].maxRadius()) {
      explosions.remove(i);
      i--;
    }

  // Update all the moving objects

  for (int i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (int i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (int i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );

  // Check for explosions overlapping silos and cities
  //
  // YOUR CODE HERE (Step 5c)
  for (int i = 0; i < explosions.size(); ++i) {
    if (citiesLeftAlive > 0) {
      for (int j = 0; j < cities.size(); ++j) {
        if (cities[j].isAlive() && explosions[i].overlaps(cities[j].position())) {
          cities.remove(j);
          citiesLeftAlive--;
        }
      }
    }

    if (silos.size() > 0) {
      for (int j = 0; j < silos.size(); ++j) {
        if (silos[j].isAlive() && explosions[i].overlaps(silos[j].position())) {
          silos[j].deactivate();
        }
      }
    }
  } 


  // Check for outgoing missiles that have exploded and intersect an
  // incoming missile

  for (int i=0; i<explosions.size(); i++) {
    if (explosions[i].isOutgoingMissile()) {

      // YOUR CODE HERE (Step 5d)
      if (missilesIn.size() > 0) {
        for (int j = 0; j < missilesIn.size(); ++j) {
          if(explosions[i].overlaps(missilesIn[j].position())) {
            missilesIn.remove(j);
            score++;
          }
        }
      }
    }
  }

  // Check for advancing to next round (e.g. when a certain number of
  // incoming missile have finished, or when all cities are destroyed).

  if (cities.size() == 0)
    pauseGame = true;
}


// Fire a missile


void World::fireMissile( int siloIndex, vec3 worldMousePos )

{
  // YOUR CODE HERE (Step 2)
  if (silos[siloIndex].canShoot()) {
    silos[siloIndex].decrMissiles();

    vec3 initPosition = silos[siloIndex].position();
    vec3 initVelocity = INIT_OUTGOING_MISSILE_SPEED * (vec3(worldMousePos.x, worldMousePos.y, 0) - silos[siloIndex].position()).normalize();
    float destY = worldMousePos.y;

    missilesOut.add(Missile(initPosition, initVelocity, destY, vec3(1, 1, 0)));
  }
}



// Initialize the world.
//
// Should do things differently for rounds 1,2,3,etc.
//
// Currently, the round is ignored.



void World::initWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;         // seconds for incoming missile to travel from top to bottom
  scorePerDestroyedMissile = 100;

  // Seed the random number generator

#ifdef LINUX
  struct timeval t;
  gettimeofday( &t, NULL );
  srand( t.tv_sec );
#else
  srand( 12345 );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vec3( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vec3( 0.2, 0, 0 ) ) );
  cities.add( City( vec3( 0.3, 0, 0 ) ) );
  cities.add( City( vec3( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vec3( 0.6, 0, 0 ) ) );
  cities.add( City( vec3( 0.7, 0, 0 ) ) );
  cities.add( City( vec3( 0.8, 0, 0 ) ) );

  citiesLeftAlive = cities.size();
}



// Define basic shaders


char *World::vertexShader = R"XX(

  #version 300 es
  
  uniform mat4 M;

  layout (location = 0) in vec4 position_in;
  layout (location = 1) in vec3 colour_in;

  smooth out mediump vec3 colour;
  
  void main() {
    gl_Position = M * position_in;
    colour = colour_in;
  }
)XX";


char *World::fragmentShader = R"XX(

  #version 300 es

  smooth in mediump vec3 colour;

  out mediump vec4 fragColour;
  
  void main() {
    fragColour = vec4( colour, 1.0 );
  }
)XX";
