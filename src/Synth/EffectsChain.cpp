#include "EffectsChain.h"

void EffectsChain::setup(float sampleRate) {
    auto filter = std::make_unique<LowPassFilter>();
    filter->setup(sampleRate);
    filter->setCutoff(8000.f); // default: mostly open, gentle high-end rolloff
    filterPtr = filter.get();
    effects.push_back(std::move(filter));

    auto delay = std::make_unique<DelayEffect>();
    delay->setup(sampleRate);
    delayPtr = delay.get();
    effects.push_back(std::move(delay));
}

float EffectsChain::process(float input) {
    float sample = input;
    for (auto& fx : effects) {
        if (!fx->isBypassed()) {
            sample = fx->process(sample);
        }
    }
    return sample;
}

void EffectsChain::setFilterCutoff(float hz) {
    if (filterPtr) filterPtr->setCutoff(hz);
}
void EffectsChain::setFilterBypassed(bool b) {
    if (filterPtr) filterPtr->setBypassed(b);
}
void EffectsChain::setDelayTime(float seconds) {
    if (delayPtr) delayPtr->setDelayTime(seconds);
}
void EffectsChain::setDelayFeedback(float fb) {
    if (delayPtr) delayPtr->setFeedback(fb);
}
void EffectsChain::setDelayBypassed(bool b) {
    if (delayPtr) delayPtr->setBypassed(b);
}
