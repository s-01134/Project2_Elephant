#pragma once
#include "VoiceManager.h"

// SynthEngine is the audio "brain" of the app, running on the audio
// thread. From iteration 2 it delegates all voice-pool management to
// VoiceManager -- still COMPOSITION (SynthEngine owns the manager),
// just moved one level down now that voice allocation logic has
// grown enough to deserve its own class.
class SynthEngine {
public:
    void setup(float sampleRate);
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);

    // Fills an audio buffer -- called from ofApp::audioOut every block.
    void render(float* outputBuffer, int numFrames, int numChannels);

private:
    VoiceManager voiceManager;
};
