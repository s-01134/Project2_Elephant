#include "OscillatorVoice.h"
#include "ofMain.h" // for TWO_PI

float OscillatorVoice::midiToFrequency(int note) const {
    return 440.f * powf(2.f, (note - 69) / 12.f);
}

void OscillatorVoice::onNoteOn(int note, float velocity) {
    amplitude = velocity;
    phase = 0.f;
    phaseIncrement = (midiToFrequency(note) / sampleRate) * TWO_PI;
}

float OscillatorVoice::generateRawSample() {
    float sample = sinf(phase) * amplitude;
    phase += phaseIncrement;
    if (phase > TWO_PI) phase -= TWO_PI;
    return sample;
}
