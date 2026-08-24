#pragma once
#include "Effect.h"

// A simple one-pole IIR low-pass filter. Smooths out high frequencies
// above the cutoff -- the classic "muffled" synth filter sweep sound.
class LowPassFilter : public Effect {
public:
    void setup(float sampleRate);
    void setCutoff(float hz);

    float process(float input) override;

private:
    float sampleRate = 48000.f;
    float cutoffHz = 4000.f;
    float alpha = 1.f; // smoothing coefficient, recalculated whenever cutoff/sampleRate change
    float previousOutput = 0.f;

    void recalcAlpha();
};
