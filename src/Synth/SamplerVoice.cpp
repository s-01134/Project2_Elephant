#include "SamplerVoice.h"
#include <cmath>
#include <algorithm>

void SamplerVoice::onNoteOn(int midiNote, float velocity) {
    amplitude = velocity;
    readPosition = 0.0;
    // Each semitone away from BASE_MIDI_NOTE scales playback speed by
    // 2^(1/12) -- the same ratio used for pitched oscillators, just
    // applied to sample-read speed instead of a phase increment.
    playbackRate = std::pow(2.0, (midiNote - BASE_MIDI_NOTE) / 12.0);
}

float SamplerVoice::generateRawSample() {
    if (!sampleData || sampleData->numFrames == 0) {
        return 0.f; // no sample loaded -- silent rather than crashing
    }

    int numFrames = sampleData->numFrames;

    if (readPosition >= numFrames) {
        return 0.f; // reached the end of the sample; envelope release finishes naturally
    }

    // Linear interpolation between the two nearest sample frames --
    // avoids the crackle of simply truncating to an integer index
    // whenever playbackRate isn't exactly 1.0.
    int index0 = static_cast<int>(readPosition);
    int index1 = std::min(index0 + 1, numFrames - 1);
    float frac = static_cast<float>(readPosition - index0);

    float sample0 = sampleData->getSample(index0, 0); // channel 0
    float sample1 = sampleData->getSample(index1, 0);
    float sample = sample0 + (sample1 - sample0) * frac;

    readPosition += playbackRate;
    return sample * amplitude;
}
