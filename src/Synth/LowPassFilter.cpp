#include "LowPassFilter.h"
#include <algorithm>

namespace {
    // Defined locally rather than relying on M_PI -- MSVC doesn't
    // define it by default, which already cost a build error once
    // this project (in Spectrum.cpp).
    constexpr float PI_F = 3.14159265358979323846f;
}

void LowPassFilter::setup(float sr) {
    sampleRate = sr;
    recalcAlpha();
}

void LowPassFilter::setCutoff(float hz) {
    cutoffHz = std::max(hz, 20.f); // clamp above 0 to avoid divide issues
    recalcAlpha();
}

void LowPassFilter::recalcAlpha() {
    float rc = 1.f / (2.f * PI_F * std::max(cutoffHz, 20.f));
    float dt = 1.f / sampleRate;
    alpha = dt / (rc + dt);
}

float LowPassFilter::process(float input) {
    previousOutput = previousOutput + alpha * (input - previousOutput);
    return previousOutput;
}
