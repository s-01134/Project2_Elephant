#pragma once
#include "SynthVoice.h"

// One of four interchangeable waveform voices (Sine/Saw/Square/Noise),
// all siblings under SynthVoice -- this is the INHERITANCE
// relationship: VoiceManager can hold and mix any combination of
// these without ever knowing which concrete type it's talking to.
class SineVoice : public SynthVoice {
public:
    // Bypasses the usual MIDI-note-based pitch calculation for
    // continuous glide control (used by the theremin-style mouse
    // pad in iteration 4). Safe to call while the voice is active.
    void setFrequencyDirectly(float hz) { phaseIncrement = (hz / sampleRate) * TWO_PI_CONST; }
    void setAmplitudeDirectly(float amp) { amplitude = amp; }

protected:
    void onNoteOn(int midiNote, float velocity) override;
    float generateRawSample() override;

private:
    float phase = 0.f;
    float phaseIncrement = 0.f;
    float amplitude = 0.f;

    static constexpr float TWO_PI_CONST = 6.28318530718f;
};
