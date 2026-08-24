#pragma once

// Abstract base class for a single audio effect in the chain.
// EffectsChain holds these polymorphically (INHERITANCE) so new
// effect types can be added later without EffectsChain's processing
// loop ever needing to change.
class Effect {
public:
    virtual ~Effect() = default;
    virtual float process(float input) = 0;

    void setBypassed(bool b) { bypassed = b; }
    bool isBypassed() const { return bypassed; }

protected:
    bool bypassed = false;
};
