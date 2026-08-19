#include "OscillatorVoice.h"
#include "ofMain.h" // for TWO_PI

void OscillatorVoice::setup(float sr) {
    sampleRate = sr;
}

float OscillatorVoice::midiToFrequency(int note) const {
    return 440.f * powf(2.f, (note - 69) / 12.f);
}

void OscillatorVoice::noteOn(int note, float velocity) {
    midiNote = note;
    amplitude = velocity;
    phase = 0.f;
    phaseIncrement = (midiToFrequency(note) / sampleRate) * TWO_PI;
    active = true;
}

void OscillatorVoice::noteOff() {
    // Iteration 2 fades this out via an ADSR envelope instead of
    // cutting the sound instantly, which currently causes a click.
    active = false;
}

float OscillatorVoice::renderSample() {
    if (!active) return 0.f;
    float sample = sinf(phase) * amplitude;
    phase += phaseIncrement;
    if (phase > TWO_PI) phase -= TWO_PI;
    return sample;
}
