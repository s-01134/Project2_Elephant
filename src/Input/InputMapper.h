#pragma once
#include <map>
#include "../Synth/SynthEngine.h"
#include "../Synth/VoiceManager.h"

// InputMapper runs on the MAIN thread only. It converts raw
// openFrameworks keyboard (and, from iteration 6, mouse) events into
// note numbers and calls SynthEngine::noteOn/noteOff, and interprets
// a separate set of keys as voice-type-select commands.
//
// It holds a non-owning pointer to the engine it talks to -- an
// ASSOCIATION relationship ("uses, does not own"): InputMapper does
// not create the SynthEngine, does not destroy it, and must not
// outlive it. ofApp owns both and guarantees that lifetime order.
class InputMapper {
public:
    void setup(SynthEngine* engine);
    void keyPressed(int key);
    void keyReleased(int key);

private:
    SynthEngine* synthEngine = nullptr; // association, not owned
    std::map<int, int> keyToMidiNote;
    std::map<int, VoiceManager::VoiceType> voiceTypeKeyMap;
};
