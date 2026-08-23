#include "SynthEngine.h"
#include <algorithm>

void SynthEngine::setup(float sr) {
    voiceManager.setup(sr, 2); // 2 voices per type x 5 types = 10 total
    thereminVoice.setup(sr);
    ringBuffer.setup(1024); // ~21ms at 48kHz -- enough for a couple periods of most notes
}

void SynthEngine::setVoiceType(VoiceManager::VoiceType type) {
    voiceManager.setVoiceType(type);
}

void SynthEngine::noteOn(int midiNote, float velocity) {
    voiceManager.noteOn(midiNote, velocity);
}

void SynthEngine::noteOff(int midiNote) {
    voiceManager.noteOff(midiNote);
}

void SynthEngine::theraminNoteOn(float frequencyHz, float velocity) {
    // noteOn(60, ...) triggers the envelope and sets an initial pitch
    // via the usual MIDI-note path; setFrequencyDirectly immediately
    // overrides it with the exact continuous frequency requested.
    thereminVoice.noteOn(60, velocity);
    thereminVoice.setFrequencyDirectly(frequencyHz);
}

void SynthEngine::theraminSetFrequency(float frequencyHz) {
    thereminVoice.setFrequencyDirectly(frequencyHz);
}

void SynthEngine::theraminSetAmplitude(float amplitude) {
    thereminVoice.setAmplitudeDirectly(amplitude);
}

void SynthEngine::theraminNoteOff() {
    thereminVoice.noteOff();
}

void SynthEngine::render(float* outputBuffer, int numFrames, int numChannels) {
    int n = std::min(numFrames, static_cast<int>(scratchBlock.size()));

    for (int i = 0; i < n; ++i) {
        float mixed = voiceManager.renderSample() + thereminVoice.renderSample() * 0.6f;
        scratchBlock[i] = mixed;
        for (int ch = 0; ch < numChannels; ++ch) {
            outputBuffer[i * numChannels + ch] = mixed;
        }
    }

    ringBuffer.write(scratchBlock.data(), n);
}
