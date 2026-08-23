#pragma once
#include "ofMain.h"
#include "Synth/SynthEngine.h"
#include "Input/InputMapper.h"
#include "Input/InteractionPad.h"
#include "Visualization/Visualizer.h"

// ofApp owns SynthEngine, InputMapper, InteractionPad, and Visualizer
// (composition) and is responsible for their relative lifetime:
// synthEngine is declared first, so it is constructed before the
// others and destroyed after them -- keeping every association
// pointer (InputMapper/InteractionPad -> SynthEngine,
// Visualizer -> AudioRingBuffer) valid for its entire life.
class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void audioOut(ofSoundBuffer &buffer) override;
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mousePressed(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;

private:
    SynthEngine synthEngine;
    InputMapper inputMapper;
    InteractionPad interactionPad;
    Visualizer visualizer;
    ofSoundStream soundStream;

    // Layout for the interaction pad region, drawn and hit-tested
    // against these bounds.
    static constexpr float PAD_X = 20.f;
    static constexpr float PAD_Y = 260.f;
    static constexpr float PAD_WIDTH = 600.f;
    static constexpr float PAD_HEIGHT = 200.f;
};
