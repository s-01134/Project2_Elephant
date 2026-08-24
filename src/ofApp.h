#pragma once
#include <array>
#include "ofMain.h"
#include "Synth/SynthEngine.h"
#include "Input/InputMapper.h"
#include "Input/InteractionPad.h"
#include "Visualization/Visualizer.h"
#include "GUI/KnobControl.h"

// ofApp owns SynthEngine, InputMapper, InteractionPad, Visualizer,
// and the 8 KnobControls (composition), and is responsible for their
// relative lifetime: synthEngine is declared first, so it is
// constructed before the others and destroyed after them -- keeping
// every association pointer (InputMapper/InteractionPad -> SynthEngine,
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

    // The 8 continuous parameters exposed as knobs: ADSR x4, filter
    // cutoff, delay time, delay feedback, master gain.
    KnobControl attackKnob, decayKnob, sustainKnob, releaseKnob;
    KnobControl cutoffKnob, delayTimeKnob, delayFeedbackKnob, gainKnob;
    std::array<KnobControl*, 8> allKnobs;
    KnobControl* activeKnob = nullptr; // which knob (if any) is currently being dragged

    void updateEnvelopeFromKnobs();
    void onEffectOrGainKnobChanged();

    // Layout for the interaction pad region.
    static constexpr float PAD_X = 20.f;
    static constexpr float PAD_Y = 260.f;
    static constexpr float PAD_WIDTH = 600.f;
    static constexpr float PAD_HEIGHT = 190.f;
};
