#pragma once
#include "SynthVoice.h"

// One of four interchangeable waveform voices (Sine/Saw/Square/Noise),
// all siblings under SynthVoice -- this is the INHERITANCE
// relationship: VoiceManager can hold and mix any combination of
// these without ever knowing which concrete type it's talking to.
class SineVoice : public SynthVoice {
protected:
    void onNoteOn(int midiNote, float velocity) override;
    float generateRawSample() override;

private:
    float phase = 0.f;
    float phaseIncrement = 0.f;
    float amplitude = 0.f;
};
