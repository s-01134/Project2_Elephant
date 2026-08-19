#pragma once
#include "SynthVoice.h"

// Concrete voice type #1: a simple sine oscillator.
// Iteration 3 turns this into a small family (Sine / Saw / Square /
// Noise) that all inherit from SynthVoice, giving us the "3+ distinct
// sounds" requirement through polymorphism rather than if/else branching.
class OscillatorVoice : public SynthVoice {
public:
    void setup(float sampleRate);

    void noteOn(int midiNote, float velocity) override;
    void noteOff() override;
    float renderSample() override;
    bool isActive() const override { return active; }

private:
    float sampleRate = 44100.f;
    float phase = 0.f;
    float phaseIncrement = 0.f;
    float amplitude = 0.f;

    float midiToFrequency(int note) const;
};
