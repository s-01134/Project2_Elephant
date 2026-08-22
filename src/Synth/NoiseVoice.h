#pragma once
#include "SynthVoice.h"

// The one voice type in this family that ignores pitch entirely --
// white noise, useful for percussive/unpitched sounds. Still gets a
// full ADSR envelope for free, inherited from the SynthVoice base.
class NoiseVoice : public SynthVoice {
protected:
    void onNoteOn(int midiNote, float velocity) override;
    float generateRawSample() override;

private:
    float amplitude = 0.f;
};
