#pragma once
#include "SynthVoice.h"

class SawVoice : public SynthVoice {
protected:
    void onNoteOn(int midiNote, float velocity) override;
    float generateRawSample() override;

private:
    float phase = 0.f;
    float phaseIncrement = 0.f;
    float amplitude = 0.f;
};
