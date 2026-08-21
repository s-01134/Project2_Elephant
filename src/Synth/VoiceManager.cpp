#include "VoiceManager.h"

void VoiceManager::setup(float sampleRate, int numVoices) {
    voices.clear();
    voiceAge.assign(numVoices, 0);
    for (int i = 0; i < numVoices; ++i) {
        // Iteration 3 will make this pool heterogeneous (a mix of
        // waveform types); for now every voice is a plain sine.
        auto voice = std::make_unique<OscillatorVoice>();
        voice->setup(sampleRate);
        voices.push_back(std::move(voice));
    }
}

SynthVoice* VoiceManager::findVoicePlaying(int midiNote) {
    for (auto& v : voices) {
        if (v->isActive() && v->getMidiNote() == midiNote) return v.get();
    }
    return nullptr;
}

int VoiceManager::findFreeVoiceIndex() {
    for (size_t i = 0; i < voices.size(); ++i) {
        if (!voices[i]->isActive()) return static_cast<int>(i);
    }
    return -1;
}

int VoiceManager::findOldestVoiceIndex() {
    int oldestIndex = 0;
    int oldestAge = voiceAge[0];
    for (size_t i = 1; i < voices.size(); ++i) {
        if (voiceAge[i] < oldestAge) {
            oldestAge = voiceAge[i];
            oldestIndex = static_cast<int>(i);
        }
    }
    return oldestIndex;
}

void VoiceManager::noteOn(int midiNote, float velocity) {
    int index = findFreeVoiceIndex();
    if (index == -1) {
        index = findOldestVoiceIndex(); // voice stealing
    }
    voices[index]->noteOn(midiNote, velocity);
    voiceAge[index] = ++ageCounter;
}

void VoiceManager::noteOff(int midiNote) {
    if (auto* v = findVoicePlaying(midiNote)) {
        v->noteOff();
    }
}

float VoiceManager::renderSample() {
    float mixed = 0.f;
    for (auto& v : voices) {
        mixed += v->renderSample();
    }
    return mixed * 0.2f; // headroom for up to ~8 simultaneous voices
}
