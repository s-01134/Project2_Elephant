#pragma once
#include "ofMain.h"
#include "../Synth/AudioRingBuffer.h"

// Draws a live oscilloscope + spectrum analyzer from audio data
// published by SynthEngine. Runs entirely on the MAIN thread.
//
// Holds a non-owning pointer to the ring buffer it reads from --
// ASSOCIATION, the same relationship InputMapper has with SynthEngine
// and SamplerVoice has with its shared sample data.
class Visualizer {
public:
    void setup(const AudioRingBuffer* buffer);
    void draw(float x, float y, float width, float height);

private:
    const AudioRingBuffer* ringBuffer = nullptr; // association, not owned
};
