#include "VoiceManager.h"
#include <climits>
#include <iostream>

void VoiceManager::setup(float sampleRate, int voicesPerType) {
    voices.clear();
    voiceKind.clear();
    voiceAge.clear();

    // Load the sample once here, using our own dependency-free WAV
    // reader (see WavLoader.h/.cpp) rather than an openFrameworks
    // sound-loading class -- avoids further build friction from
    // sound-API differences across oF versions/installs. If it's
    // missing, we log and carry on with an empty buffer; SamplerVoice
    // checks for that and plays silence instead of crashing.
    // Our loader bypasses openFrameworks' path resolution (which
    // normally adds a "data/" prefix to relative paths automatically),
    // so try several likely locations rather than assuming one -- the
    // app's actual working directory at runtime varies by VS project
    // configuration.
    const char* candidatePaths[] = {
        "kick.wav",
        "data/kick.wav",
        "bin/data/kick.wav",
        "../data/kick.wav",
    };
    bool loaded = false;
    for (const char* path : candidatePaths) {
        if (loadWavFile(path, sampleData)) {
            std::cerr << "[VoiceManager] Loaded sample from: " << path << "\n";
            loaded = true;
            break;
        }
    }
    if (!loaded) {
        std::cerr << "[VoiceManager] Could not find kick.wav in any expected "
                     "location (tried: kick.wav, data/kick.wav, bin/data/kick.wav, "
                     "../data/kick.wav) -- SamplerVoice will be silent.\n";
    }

    auto addVoicesOfType = [&](VoiceType type, auto makeVoice) {
        for (int i = 0; i < voicesPerType; ++i) {
            auto voice = makeVoice();
            voice->setup(sampleRate);
            voices.push_back(std::move(voice));
            voiceKind.push_back(type);
            voiceAge.push_back(0);
        }
    };

    addVoicesOfType(VoiceType::Sine,   [] { return std::make_unique<SineVoice>(); });
    addVoicesOfType(VoiceType::Saw,    [] { return std::make_unique<SawVoice>(); });
    addVoicesOfType(VoiceType::Square, [] { return std::make_unique<SquareVoice>(); });
    addVoicesOfType(VoiceType::Noise,  [] { return std::make_unique<NoiseVoice>(); });

    // Sampler voices need the shared sample pointer wired up right
    // after construction, before they can be used -- hence the
    // slightly different lambda body from the four waveform types.
    addVoicesOfType(VoiceType::Sampler, [this] {
        auto voice = std::make_unique<SamplerVoice>();
        voice->setSampleData(&sampleData);
        return voice;
    });
}

SynthVoice* VoiceManager::findVoicePlaying(int midiNote) {
    // Deliberately searches ALL voices, regardless of type: a note
    // may have been triggered on a voice type that isn't the
    // currently selected one anymore, and noteOff must still find it.
    for (auto& v : voices) {
        if (v->isActive() && v->getMidiNote() == midiNote) return v.get();
    }
    return nullptr;
}

int VoiceManager::findFreeVoiceIndex() {
    for (size_t i = 0; i < voices.size(); ++i) {
        if (voiceKind[i] == currentType && !voices[i]->isActive()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int VoiceManager::findOldestVoiceIndex() {
    int oldestIndex = -1;
    int oldestAge = INT_MAX;
    for (size_t i = 0; i < voices.size(); ++i) {
        if (voiceKind[i] == currentType && voiceAge[i] < oldestAge) {
            oldestAge = voiceAge[i];
            oldestIndex = static_cast<int>(i);
        }
    }
    return oldestIndex;
}

void VoiceManager::setEnvelopeParams(float attack, float decay, float sustain, float release) {
    for (auto& v : voices) {
        v->setEnvelopeParams(attack, decay, sustain, release);
    }
}

void VoiceManager::noteOn(int midiNote, float velocity) {
    int index = findFreeVoiceIndex();
    if (index == -1) {
        index = findOldestVoiceIndex(); // voice stealing, within the current type's pool
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
    return mixed * 0.2f; // headroom
}
