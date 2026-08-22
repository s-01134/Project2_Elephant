#include "SawVoice.h"
#include "MidiUtils.h"
#include "ofMain.h" // for TWO_PI

void SawVoice::onNoteOn(int midiNote, float velocity) {
    amplitude = velocity;
    phase = 0.f;
    phaseIncrement = (midiNoteToFrequency(midiNote) / sampleRate) * TWO_PI;
}

float SawVoice::generateRawSample() {
    // Ramps linearly from -1 to 1 across one period -- a naive
    // (non-band-limited) sawtooth. Good enough for this project's
    // scope; a production synth would band-limit this to avoid
    // aliasing on high notes.
    float sample = ((phase / TWO_PI) * 2.f - 1.f) * amplitude;
    phase += phaseIncrement;
    if (phase > TWO_PI) phase -= TWO_PI;
    return sample;
}
