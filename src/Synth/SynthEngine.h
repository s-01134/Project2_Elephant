#pragma once
#include <vector>
#include <memory>
#include "SynthVoice.h"
#include "OscillatorVoice.h"

// SynthEngine is the audio "brain" of the app. ofApp just forwards
// OF lifecycle events to it, keeping audio logic out of the app class.
//
// SynthEngine OWNS its voices (std::unique_ptr) -- this is the
// COMPOSITION relationship: the voices' lifetime is entirely
// controlled by the engine, and they are destroyed when it is.
class SynthEngine {
public:
    void setup(float sampleRate);
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);

    // Fills an audio buffer -- called from ofApp::audioOut every block.
    void render(float* outputBuffer, int numFrames, int numChannels);

private:
    float sampleRate = 44100.f;
    std::vector<std::unique_ptr<SynthVoice>> voices;

    SynthVoice* findVoicePlaying(int midiNote);
};
