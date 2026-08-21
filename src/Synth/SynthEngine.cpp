#include "SynthEngine.h"

void SynthEngine::setup(float sr) {
    voiceManager.setup(sr, 8); // 8-voice polyphony (up from 4 in iteration 1)
}

void SynthEngine::noteOn(int midiNote, float velocity) {
    voiceManager.noteOn(midiNote, velocity);
}

void SynthEngine::noteOff(int midiNote) {
    voiceManager.noteOff(midiNote);
}

void SynthEngine::render(float* outputBuffer, int numFrames, int numChannels) {
    for (int i = 0; i < numFrames; ++i) {
        float mixed = voiceManager.renderSample();
        for (int ch = 0; ch < numChannels; ++ch) {
            outputBuffer[i * numChannels + ch] = mixed;
        }
    }
}
