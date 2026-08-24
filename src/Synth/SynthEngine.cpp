#include "SynthEngine.h"
#include <algorithm>

void SynthEngine::setup(float sr) {
    voiceManager.setup(sr, 2); // 2 voices per type x 5 types = 10 total
    thereminVoice.setup(sr);
    effectsChain.setup(sr);
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

void SynthEngine::thereminNoteOn(float frequencyHz, float velocity) {
    // noteOn(60, ...) triggers the envelope and sets an initial pitch
    // via the usual MIDI-note path; setFrequencyDirectly immediately
    // overrides it with the exact continuous frequency requested.
    thereminVoice.noteOn(60, velocity);
    thereminVoice.setFrequencyDirectly(frequencyHz);
}

void SynthEngine::thereminSetFrequency(float frequencyHz) {
    thereminVoice.setFrequencyDirectly(frequencyHz);
}

void SynthEngine::thereminSetAmplitude(float amplitude) {
    thereminVoice.setAmplitudeDirectly(amplitude);
}

void SynthEngine::thereminNoteOff() {
    thereminVoice.noteOff();
}

void SynthEngine::setEnvelopeParams(float attack, float decay, float sustain, float release) {
    voiceManager.setEnvelopeParams(attack, decay, sustain, release);
    thereminVoice.setEnvelopeParams(attack, decay, sustain, release);
}

void SynthEngine::setFilterCutoff(float hz) {
    effectsChain.setFilterCutoff(hz);
}

void SynthEngine::setDelayTime(float seconds) {
    effectsChain.setDelayTime(seconds);
}

void SynthEngine::setDelayFeedback(float feedback) {
    effectsChain.setDelayFeedback(feedback);
}

void SynthEngine::setMasterGain(float gain) {
    masterGain = std::clamp(gain, 0.f, 1.f);
}

void SynthEngine::render(float* outputBuffer, int numFrames, int numChannels) {
    int n = std::min(numFrames, static_cast<int>(scratchBlock.size()));

    for (int i = 0; i < n; ++i) {
        float mixed = voiceManager.renderSample() + thereminVoice.renderSample() * 0.6f;
        mixed = effectsChain.process(mixed);
        mixed *= masterGain;
        scratchBlock[i] = mixed;
        for (int ch = 0; ch < numChannels; ++ch) {
            outputBuffer[i * numChannels + ch] = mixed;
        }
    }

    ringBuffer.write(scratchBlock.data(), n);
}
