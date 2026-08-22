#include "SquareVoice.h"
#include "MidiUtils.h"
#include "ofMain.h" // for TWO_PI

void SquareVoice::onNoteOn(int midiNote, float velocity) {
    amplitude = velocity;
    phase = 0.f;
    phaseIncrement = (midiNoteToFrequency(midiNote) / sampleRate) * TWO_PI;
}

float SquareVoice::generateRawSample() {
    float sample = (sinf(phase) >= 0.f ? 1.f : -1.f) * amplitude;
    phase += phaseIncrement;
    if (phase > TWO_PI) phase -= TWO_PI;
    return sample;
}
