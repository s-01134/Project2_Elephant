#pragma once
#include "SynthVoice.h"

// Concrete voice type: a sine oscillator. Only implements the two
// hooks SynthVoice requires -- envelope handling, active-state
// tracking, etc. all now live in the base class.
//
// Iteration 3 adds SawVoice / SquareVoice / NoiseVoice as siblings of
// this class, all inheriting directly from SynthVoice the same way.
class OscillatorVoice : public SynthVoice {
protected:
    void onNoteOn(int midiNote, float velocity) override;
    float generateRawSample() override;

private:
    float phase = 0.f;
    float phaseIncrement = 0.f;
    float amplitude = 0.f;

    float midiToFrequency(int note) const;
};
