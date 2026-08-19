#include "SynthEngine.h"

void SynthEngine::setup(float sr) {
    sampleRate = sr;
    // Iteration 1: a small fixed pool of oscillator voices, allocated
    // up front so no audio-thread memory allocation ever happens.
    // Iteration 2 replaces the allocation logic below with a proper
    // VoiceManager class that also handles voice stealing cleanly.
    const int NUM_VOICES = 4;
    for (int i = 0; i < NUM_VOICES; ++i) {
        auto voice = std::make_unique<OscillatorVoice>();
        voice->setup(sampleRate);
        voices.push_back(std::move(voice));
    }
}

SynthVoice* SynthEngine::findVoicePlaying(int midiNote) {
    for (auto& v : voices) {
        if (v->isActive() && v->getMidiNote() == midiNote) return v.get();
    }
    return nullptr;
}

void SynthEngine::noteOn(int midiNote, float velocity) {
    // Grab the first inactive voice.
    for (auto& v : voices) {
        if (!v->isActive()) {
            v->noteOn(midiNote, velocity);
            return;
        }
    }
    // All voices busy: steal the first one (crude, but fine for now).
    voices.front()->noteOn(midiNote, velocity);
}

void SynthEngine::noteOff(int midiNote) {
    if (auto* v = findVoicePlaying(midiNote)) {
        v->noteOff();
    }
}

void SynthEngine::render(float* outputBuffer, int numFrames, int numChannels) {
    for (int i = 0; i < numFrames; ++i) {
        float mixed = 0.f;
        for (auto& v : voices) {
            mixed += v->renderSample();
        }
        mixed *= 0.25f; // headroom so 4 summed voices don't clip
        for (int ch = 0; ch < numChannels; ++ch) {
            outputBuffer[i * numChannels + ch] = mixed;
        }
    }
}
