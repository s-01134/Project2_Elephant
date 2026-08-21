#pragma once
#include <vector>
#include <memory>
#include "SynthVoice.h"
#include "OscillatorVoice.h"

// VoiceManager owns a fixed pool of voices (COMPOSITION -- same
// relationship SynthEngine used in iteration 1, just moved to a
// dedicated class with its own single responsibility) and handles
// real polyphonic allocation: reuse a free voice if one exists,
// otherwise steal the OLDEST currently-sounding voice, rather than
// always grabbing voices.front() like iteration 1 did.
class VoiceManager {
public:
    void setup(float sampleRate, int numVoices = 8);
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);

    // Sums and returns one mixed sample from every voice in the pool.
    float renderSample();

private:
    std::vector<std::unique_ptr<SynthVoice>> voices;
    std::vector<int> voiceAge; // higher = more recently triggered
    int ageCounter = 0;

    SynthVoice* findVoicePlaying(int midiNote);
    int findFreeVoiceIndex();
    int findOldestVoiceIndex();
};
