#pragma once
#include "ofMain.h"
#include "Synth/SynthEngine.h"
#include "Input/InputMapper.h"

// ofApp owns both SynthEngine and InputMapper (composition) and is
// responsible for their relative lifetime: synthEngine is declared
// first, so it is constructed before inputMapper and destroyed after
// it, which keeps InputMapper's association pointer valid for its
// entire life.
class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void audioOut(ofSoundBuffer &buffer) override;
    void keyPressed(int key) override;
    void keyReleased(int key) override;

private:
    SynthEngine synthEngine;
    InputMapper inputMapper;
    ofSoundStream soundStream;
};
