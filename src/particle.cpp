#include "particle.hpp"
#include "ofApp.h"

Particle::Particle(ofVec2f position, float lifetime, unsigned int sliceIndex, ofColor color)
{
    mPosition = position;
    mVelocity = ofVec2f(0.0f, 0.0f);
    mLifetime = lifetime;
    mAge = 0.0f;
    mIsDead = false;
    mSliceIndex = sliceIndex;
    mColor = color;
    mIsPenDown = true;
}

void Particle::update(ofVec3f speed, ofVec2f multiplier, ofVec2f friction)
{

    if ((int)mAge % 20 == 0)
    {
        mIsPenDown = !mIsPenDown;
    }

    if (mAge > mLifetime)
    {
        mIsDead = true;
    }

    speed *= 0.005;

    mNoiseFloat = ofNoise(mPosition.x * speed.x, mPosition.y * speed.y, mAge * speed.z);
    mNoiseVec.x = cos(mNoiseFloat * M_PI_2 * multiplier.x);
    mNoiseVec.y = sin(mNoiseFloat * M_PI_2 * multiplier.y);
    mVelocity += mNoiseVec;
    mVelocity *= friction;
    mPosition += mVelocity;
    mAge++;
}
