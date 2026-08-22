#pragma once
#include <vector>
#include <memory>
#include "SynthVoice.h"
#include "SineVoice.h"
#include "SawVoice.h"
#include "SquareVoice.h"
#include "NoiseVoice.h"
#include "SamplerVoice.h"
#include "WavLoader.h"

// VoiceManager owns a fixed pool of voices (COMPOSITION), split into
// five equal-sized sub-pools -- one per voice type -- all created up
// front in setup(). Switching the active voice type only changes
// which sub-pool new notes are pulled from; it never constructs or
// destroys a voice at runtime. That matters because noteOn() runs on
// the MAIN thread while renderSample() runs on the AUDIO thread --
// swapping a voice object's dynamic type live would be a data race
// between the two. Pre-allocating every type avoids that entirely.
//
// VoiceManager also owns the one shared sample buffer used by every
// SamplerVoice (composition of the data; the SamplerVoice instances
// only hold a non-owning pointer to it -- association).
class VoiceManager {
public:
    enum class VoiceType { Sine, Saw, Square, Noise, Sampler };

    void setup(float sampleRate, int voicesPerType = 2);
    void setVoiceType(VoiceType type) { currentType = type; }
    VoiceType getVoiceType() const { return currentType; }

    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);

    // Sums and returns one mixed sample from every voice in the pool.
    float renderSample();

private:
    std::vector<std::unique_ptr<SynthVoice>> voices;
    std::vector<VoiceType> voiceKind; // parallel array, set once in setup()
    std::vector<int> voiceAge;        // higher = more recently triggered
    int ageCounter = 0;
    VoiceType currentType = VoiceType::Sine;

    SampleData sampleData; // loaded once, shared by pointer with every SamplerVoice

    SynthVoice* findVoicePlaying(int midiNote);
    int findFreeVoiceIndex();   // only among voices of currentType
    int findOldestVoiceIndex(); // only among voices of currentType
};
