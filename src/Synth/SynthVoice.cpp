#include "SynthVoice.h"

void SynthVoice::setup(float sr) {
    sampleRate = sr;
    envelope.setup(sr);
}

void SynthVoice::noteOn(int note, float velocity) {
    midiNote = note;
    active = true;
    envelope.noteOn();
    onNoteOn(note, velocity);
}

void SynthVoice::noteOff() {
    envelope.noteOff();
    // 'active' deliberately stays true through the release tail --
    // the voice keeps rendering (fading out) until the envelope
    // reports idle, which is checked in renderSample() below.
}

float SynthVoice::renderSample() {
    if (!active) return 0.f;

    float raw = generateRawSample();
    float env = envelope.process();

    if (envelope.isIdle()) {
        active = false; // now safe for VoiceManager to reuse this voice
    }

    return raw * env;
}
