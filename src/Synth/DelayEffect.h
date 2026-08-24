#pragma once
#include "Effect.h"
#include <vector>

// A simple feedback delay ("echo"). The delay line buffer is
// allocated ONCE in setup() at a fixed maximum size -- process() runs
// on the audio thread every sample, so it must never allocate.
class DelayEffect : public Effect {
public:
    void setup(float sampleRate, float maxDelaySeconds = 2.f);
    void setDelayTime(float seconds);
    void setFeedback(float feedback);
    void setMix(float mix);

    float process(float input) override;

private:
    float sampleRate = 48000.f;
    std::vector<float> buffer; // fixed-size circular delay line, allocated once
    int writeIndex = 0;
    int delaySamples = 0;
    float feedback = 0.35f;
    float mix = 0.3f; // 0 = fully dry, 1 = fully wet
};
