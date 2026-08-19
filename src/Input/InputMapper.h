#pragma once
#include <map>
#include "../Synth/SynthEngine.h"

// InputMapper runs on the MAIN thread only. It converts raw
// openFrameworks keyboard (and, from iteration 6, mouse) events into
// note numbers and calls SynthEngine::noteOn/noteOff.
//
// It holds a raw, non-owning pointer to the engine it talks to -- this
// is an ASSOCIATION relationship ("uses, does not own"): InputMapper
// does not create the SynthEngine, does not destroy it, and must not
// outlive it. ofApp owns both and guarantees that lifetime order.
class InputMapper {
public:
    void setup(SynthEngine* engine);
    void keyPressed(int key);
    void keyReleased(int key);

private:
    SynthEngine* synthEngine = nullptr; // association, not owned
    std::map<int, int> keyToMidiNote;   // iteration 2 grows this into a full scale
};
