#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle
{

public:
  Particle(ofVec2f position, float lifetime, uint sliceIndex, ofColor color);

  ofVec2f mPosition;
  ofVec2f mVelocity;
  ofVec2f mNoiseVec;
  ofColor mColor;

  float mNoiseFloat;
  float mLifetime;
  float mAge;

  unsigned int mSliceIndex;

  bool mIsDead;
  bool mIsPenDown;

  void update(ofVec3f speed, ofVec2f multiplier, ofVec2f friction);
};

#endif
