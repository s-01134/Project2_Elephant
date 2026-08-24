#include "DelayEffect.h"
#include <algorithm>

void DelayEffect::setup(float sr, float maxDelaySeconds) {
    sampleRate = sr;
    buffer.assign(static_cast<size_t>(sr * maxDelaySeconds), 0.f);
    writeIndex = 0;
    setDelayTime(0.3f); // sensible default: 300ms
}

void DelayEffect::setDelayTime(float seconds) {
    int maxIndex = static_cast<int>(buffer.size()) - 1;
    delaySamples = std::clamp(static_cast<int>(seconds * sampleRate), 1, std::max(1, maxIndex));
}

void DelayEffect::setFeedback(float fb) {
    feedback = std::clamp(fb, 0.f, 0.9f); // capped below 1.0 so it can't run away into infinite buildup
}

void DelayEffect::setMix(float m) {
    mix = std::clamp(m, 0.f, 1.f);
}

float DelayEffect::process(float input) {
    int bufSize = static_cast<int>(buffer.size());
    int readIndex = writeIndex - delaySamples;
    if (readIndex < 0) readIndex += bufSize;

    float delayedSample = buffer[readIndex];
    buffer[writeIndex] = input + delayedSample * feedback;
    writeIndex = (writeIndex + 1) % bufSize;

    return input * (1.f - mix) + delayedSample * mix;
}
