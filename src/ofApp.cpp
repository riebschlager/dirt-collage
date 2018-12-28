#include "ofApp.h"

void ofApp::onClearPressed()
{
    mCanvas.begin();
    ofBackground(0, 0, 0, 0);
    mCanvas.end();
}

void ofApp::onSaveImagePressed()
{
    saveFbo();
}

void ofApp::saveFbo()
{
    ofFbo img;
    ofPixels pixels;
    img.allocate(mCanvas.getWidth(), mCanvas.getHeight(), GL_RGBA);
    img.begin();
    ofBackground(0, 0, 0, 0);
    mCanvas.draw(0, 0);
    img.end();
    img.readToPixels(pixels);
    ofSaveImage(pixels, "output/image" + ofToString(ofGetUnixTime()) + ".tif");
}

ofVec2f ofApp::resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight)
{
    float ratio = min(maxWidth / srcWidth, maxHeight / srcHeight);
    return ofVec2f(srcWidth * ratio, srcHeight * ratio);
}

void ofApp::loadSlices(string folderName)
{
    string path = mBasePath + "slices/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        mSlices.push_back(img);
    }
}

void ofApp::loadSlices(string folderName, unsigned int maxWidth, unsigned int maxHeight)
{
    string path = mBasePath + "slices/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        ofVec2f r = resizeProportionally(img.getWidth(), img.getHeight(), maxWidth, maxHeight);
        img.resize(r.x, r.y);
        mSlices.push_back(img);
    }
}

void ofApp::loadSlicesFromSource(string folderName, unsigned int numberOfSlices, unsigned int maxWidth, unsigned int maxHeight)
{
    string path = mBasePath + "sources/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        for (unsigned int j = 0; j < numberOfSlices; j++)
        {
            ofImage slice;
            unsigned int w = ofRandom(maxWidth);
            unsigned int h = ofRandom(maxHeight);
            unsigned int x = ofRandom(0, img.getWidth() - w);
            unsigned int y = ofRandom(0, img.getHeight() - h);
            slice.cropFrom(img, x, y, w, h);
            mSlices.push_back(slice);
        }
    }
}

void ofApp::loadSources(string folderName)
{
    string path = mBasePath + "sources/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        img.resize(mCanvas.getWidth(), mCanvas.getHeight());
        mSources.push_back(img);
    }
}

void ofApp::setup()
{
    ofSetFrameRate(60);
    mIsGuiVisible = true;
    mBasePath = ofFilePath::getUserHomeDir() + "/Documents/Art/";

    mCanvas.allocate(CANVAS_WIDTH, CANVAS_HEIGHT);
    mCanvas.begin();
    ofBackground(0, 0, 0, 0);
    mCanvas.end();

    mSaveImage.addListener(this, &ofApp::onSaveImagePressed);
    mClear.addListener(this, &ofApp::onClearPressed);

    mGui.setup();

    ofVec3f minNoise(0.0f, 0.0f, 0.0f);
    ofVec3f maxNoise(1.0f, 1.0f, 1.0f);

    mGui.add(mNoiseScale.setup("Noise Scale", minNoise, minNoise, maxNoise));
    mGui.add(mNoiseOscMin.setup("Noise Osc Min", minNoise, minNoise, maxNoise));
    mGui.add(mNoiseOscMax.setup("Noise Osc Max", maxNoise, minNoise, maxNoise));
    mGui.add(mNoiseOscSpeed.setup("Noise Osc Speed", ofVec3f(100, 100, 100), ofVec3f(1, 1, 1), ofVec3f(500, 500, 500)));
    mGui.add(mIsNoiseOsc.setup("Noise Osc On", true));

    ofVec2f minMultiplier(1.0f, 1.0f);
    ofVec2f maxMultiplier(100.0f, 100.0f);

    mGui.add(mMultiplier.setup("Multiplier", minMultiplier, minMultiplier, maxMultiplier));
    mGui.add(mMultiplierOscMin.setup("Multiplier Osc Min", minMultiplier, minMultiplier, maxMultiplier));
    mGui.add(mMultiplierOscMax.setup("Multiplier Osc Max", maxMultiplier, minMultiplier, maxMultiplier));
    mGui.add(mMultiplierOscSpeed.setup("Multiplier Osc Speed", ofVec2f(100, 100), ofVec2f(1, 1), ofVec2f(500, 500)));
    mGui.add(mIsMultiplierOsc.setup("Multiplier Osc On", true));

    ofVec2f minFriction(0.0f, 0.0f);
    ofVec2f maxFriction(1.0f, 1.0f);

    mGui.add(mFriction.setup("Friction", ofVec2f(0.1f, 0.1f), minFriction, maxFriction));
    mGui.add(mFrictionOscMin.setup("Friction Osc Min", minFriction, minFriction, maxFriction));
    mGui.add(mFrictionOscMax.setup("Friction Osc Max", maxFriction, minFriction, maxFriction));
    mGui.add(mFrictionOscSpeed.setup("Friction Osc Speed", ofVec2f(100, 100), ofVec2f(1, 1), ofVec2f(500, 500)));
    mGui.add(mIsFrictionOsc.setup("Friction Osc On", true));

    mGui.add(mLifetime.setup("Lifetime", ofVec2f(20.0f, 2000.0f), ofVec2f(20.0f, 20.0f), ofVec2f(5000.0f, 5000.0f)));
    mGui.add(mScale.setup("Scale", ofVec2f(-0.5f, 0.5f), ofVec2f(-3.0f, -3.0f), ofVec2f(3.0f, 3.0f)));
    mGui.add(mRotation.setup("Rotation", ofVec2f(-45.0f, 45.0f), ofVec2f(-360.0f, -360.0f), ofVec2f(360.0f, 360.0f)));
    mGui.add(mAlpha.setup("Alpha", ofVec2f(0.0f, 255.0f), ofVec2f(0.0f, 0.0f), ofVec2f(255.0f, 255.0f)));
    mGui.add(mDrawsPerFrame.setup("Draws Per Frame", 1, 1, 1000));
    mGui.add(mIsCanvasColor.setup("Canvas Color", false));
    mGui.add(mIsRandomColor.setup("Random Color", false));
    mGui.add(mClear.setup("Clear"));
    mGui.add(mSaveImage.setup("Save Image"));

    mGui.minimizeAll();

    mCurrentSourceIndex = 0;

    loadSlices("flora", 300, 300);
    loadSources("wild");
}

void ofApp::update()
{
    if (mIsNoiseOsc)
    {
        float n = (float)ofGetFrameNum();

        mNoiseScale = ofVec3f(
            ofMap(sin(n / mNoiseOscSpeed->x), -1, 1, mNoiseOscMin->x, mNoiseOscMax->x),
            ofMap(sin(n / mNoiseOscSpeed->y), -1, 1, mNoiseOscMin->y, mNoiseOscMax->y),
            ofMap(sin(n / mNoiseOscSpeed->z), -1, 1, mNoiseOscMin->z, mNoiseOscMax->z));
    }

    if (mIsMultiplierOsc)
    {
        float n = (float)ofGetFrameNum();

        mMultiplier = ofVec2f(
            ofMap(sin(n / mMultiplierOscSpeed->x), -1, 1, mMultiplierOscMin->x, mMultiplierOscMax->x),
            ofMap(sin(n / mMultiplierOscSpeed->y), -1, 1, mMultiplierOscMin->y, mMultiplierOscMax->y));
    }

    if (mIsFrictionOsc)
    {
        float n = (float)ofGetFrameNum();

        mFriction = ofVec2f(
            ofMap(sin(n / mFrictionOscSpeed->x), -1, 1, mFrictionOscMin->x, mFrictionOscMax->x),
            ofMap(sin(n / mFrictionOscSpeed->y), -1, 1, mFrictionOscMin->y, mFrictionOscMax->y));
    }

    if (mIsDrawing)
    {
        float lifetime = ofRandom(mLifetime->x, mLifetime->y);
        unsigned int sliceIndex = ofRandom(mSlices.size());
        float x = ofMap(mMousePos.x, 0, APP_WIDTH, 0, CANVAS_WIDTH, true);
        float y = ofMap(mMousePos.y, 0, APP_HEIGHT, 0, CANVAS_HEIGHT, true);
        ofColor color;
        if (!mIsRandomColor)
        {
            color = mSources[mCurrentSourceIndex].getColor(x, y);
        }
        else
        {
            float nx = ofRandom(CANVAS_WIDTH);
            float ny = ofRandom(CANVAS_HEIGHT);
            color = mSources[mCurrentSourceIndex].getColor(nx, ny);
        }
        ofVec2f pos(
            ofMap(mMousePos.x, 0, APP_WIDTH, 0, CANVAS_WIDTH, true),
            ofMap(mMousePos.y, 0, APP_HEIGHT, 0, CANVAS_HEIGHT, true));
        mParticles.push_back(Particle(pos, lifetime, sliceIndex, color));
    }
}

void ofApp::draw()
{

    mCanvas.begin();
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    for (list<Particle>::iterator p = mParticles.begin(); p != mParticles.end();)
    {
        if (p->mIsDead)
        {
            p = mParticles.erase(p);
        }
        else
        {
            for (unsigned int i = 0; i < mDrawsPerFrame; i++)
            {
                p->update((ofVec3f)mNoiseScale, (ofVec2f)mMultiplier, (ofVec2f)mFriction);

                ofPushMatrix();
                ofTranslate(p->mPosition);
                ofScale(ofMap(p->mNoiseFloat, 0, 1, mScale->x, mScale->y, true));
                ofRotateDeg(ofMap(p->mNoiseFloat, 0, 1, mRotation->x, mRotation->y, true));
                float alpha = ofMap(p->mNoiseFloat, 0, 1, mAlpha->x, mAlpha->y, true);
                if (mIsCanvasColor)
                {
                    float x = ofClamp(p->mPosition.x, 0, mSources[mCurrentSourceIndex].getWidth());
                    float y = ofClamp(p->mPosition.y, 0, mSources[mCurrentSourceIndex].getHeight());
                    ofSetColor(mSources[mCurrentSourceIndex].getColor(x, y), alpha);
                }
                else
                {
                    ofSetColor(p->mColor, alpha);
                }

                if (p->mIsPenDown)
                {
                    mSlices[p->mSliceIndex].draw(0, 0);
                }
                ofPopMatrix();
            }
            ++p;
        }
    }
    mCanvas.end();
    ofBackground(0);
    //mSources[mCurrentSourceIndex].draw(0, 0, APP_WIDTH, APP_HEIGHT);
    mCanvas.draw(0, 0, APP_WIDTH, APP_HEIGHT);

    if (mIsGuiVisible)
    {
        mGui.draw();
    }
}

void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        for (int i = 0; i < 1000; i++)
        {
            float lifetime = ofRandom(mLifetime->x, mLifetime->y);
            unsigned int sliceIndex = ofRandom(mSlices.size());
            float x = ofRandom(CANVAS_WIDTH);
            float y = ofRandom(CANVAS_HEIGHT);
            ofColor color = mSources[mCurrentSourceIndex].getColor(x, y);
            ofVec2f pos(x, y);
            mParticles.push_back(Particle(pos, lifetime, sliceIndex, color));
        }
    }

    if (key == 'g')
    {
        mIsGuiVisible = !mIsGuiVisible;
    }

    if (key == 's')
    {
        saveFbo();
    }

    if (key == 'c')
    {
        mCanvas.begin();
        ofBackground(0, 0, 0, 0);
        mCanvas.end();
    }

    if (key == 'm')
    {
        mGui.minimizeAll();
    }

    if (key == 'n')
    {
        mGui.maximizeAll();
    }
}

void ofApp::keyReleased(int key)
{
}

void ofApp::mouseMoved(int x, int y)
{
    if (x < 0 || x > APP_WIDTH)
        return;
    if (y < 0 || y > APP_HEIGHT)
        return;
    mMousePos.set(x, y);
}

void ofApp::mouseDragged(int x, int y, int button)
{
    if (x < 0 || x > APP_WIDTH)
        return;
    if (y < 0 || y > APP_HEIGHT)
        return;
    mMousePos.set(x, y);
}

void ofApp::mousePressed(int x, int y, int button)
{
    mIsDrawing = true;
}

void ofApp::mouseReleased(int x, int y, int button)
{
    mIsDrawing = false;
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
