#pragma once

#define APP_WIDTH 1920
#define APP_HEIGHT 1080
#define CANVAS_WIDTH 3840
#define CANVAS_HEIGHT 2160

#include "ofMain.h"
#include "ofxGui.h"
#include "particle.hpp"

class ofApp : public ofBaseApp
{

public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  void saveFbo();
  void onSaveImagePressed();
  void onClearPressed();

  void loadSlices(string folderName);
  void loadSlices(string folderName, unsigned int maxWidth, unsigned int maxHeight);
  void loadSlicesFromSource(string folderName, unsigned int numberOfSlices, unsigned int maxWidth, unsigned int maxHeight);
  void loadSources(string folderName);
  ofVec2f resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight);

  vector<ofImage> mSlices;
  vector<ofImage> mSources;
  ofFbo mCanvas;
  string mBasePath;

  list<Particle> mParticles;

  bool mIsDrawing;
  bool mIsGuiVisible;
  int mCurrentSourceIndex;

  ofVec2f mMousePos;
  ofxPanel mGui;

  ofxVec3Slider mNoiseScale;
  ofxVec3Slider mNoiseOscSpeed;
  ofxVec3Slider mNoiseOscMin;
  ofxVec3Slider mNoiseOscMax;
  ofxToggle mIsNoiseOsc;

  ofxVec2Slider mMultiplier;
  ofxVec2Slider mMultiplierOscSpeed;
  ofxVec2Slider mMultiplierOscMin;
  ofxVec2Slider mMultiplierOscMax;
  ofxToggle mIsMultiplierOsc;

  ofxVec2Slider mFriction;
  ofxVec2Slider mFrictionOscSpeed;
  ofxVec2Slider mFrictionOscMin;
  ofxVec2Slider mFrictionOscMax;
  ofxToggle mIsFrictionOsc;

  ofxVec2Slider mLifetime;
  ofxVec2Slider mRotation;
  ofxVec2Slider mScale;
  ofxVec2Slider mAlpha;

  ofxToggle mIsCanvasColor;
  ofxToggle mIsRandomColor;

  ofxIntSlider mDrawsPerFrame;
  ofxButton mSaveImage;
  ofxButton mClear;
};
