#pragma once
#include <vector>
#include <memory>
#include "Effect.h"
#include "LowPassFilter.h"
#include "DelayEffect.h"

// EffectsChain owns a sequence of Effect instances (COMPOSITION) and
// processes audio through them via the polymorphic Effect interface
// (INHERITANCE) -- process() never needs to know it's specifically
// talking to a LowPassFilter or DelayEffect.
//
// It also keeps non-owning raw pointer aliases to the two concrete
// effects it constructed, purely so callers (SynthEngine, and
// eventually the GUI knobs) can adjust their specific parameters
// without EffectsChain needing a generic "setParameter(index, value)"
// interface for what is, in this project, a fixed two-effect chain.
class EffectsChain {
public:
    void setup(float sampleRate);
    float process(float input);

    void setFilterCutoff(float hz);
    void setFilterBypassed(bool bypassed);
    void setDelayTime(float seconds);
    void setDelayFeedback(float feedback);
    void setDelayBypassed(bool bypassed);

private:
    std::vector<std::unique_ptr<Effect>> effects; // processed in this order
    LowPassFilter* filterPtr = nullptr; // alias into effects[0], not owned here
    DelayEffect* delayPtr = nullptr;    // alias into effects[1], not owned here
};
