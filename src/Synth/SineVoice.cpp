#include "SineVoice.h"
#include "MidiUtils.h"
#include "ofMain.h" // for TWO_PI

void SineVoice::onNoteOn(int midiNote, float velocity) {
    amplitude = velocity;
    phase = 0.f;
    phaseIncrement = (midiNoteToFrequency(midiNote) / sampleRate) * TWO_PI;
}

float SineVoice::generateRawSample() {
    float sample = sinf(phase) * amplitude;
    phase += phaseIncrement;
    if (phase > TWO_PI) phase -= TWO_PI;
    return sample;
}
