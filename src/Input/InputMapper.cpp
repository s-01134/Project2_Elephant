#include "InputMapper.h"

void InputMapper::setup(SynthEngine* engine) {
    synthEngine = engine;
    // Iteration 1: four keys, four fixed notes. Iteration 2 replaces
    // this with a full keyboard-row-to-scale mapping.
    keyToMidiNote = {
        {'a', 60}, // C4
        {'s', 62}, // D4
        {'d', 64}, // E4
        {'f', 65}, // F4
    };
}

void InputMapper::keyPressed(int key) {
    auto it = keyToMidiNote.find(key);
    if (it != keyToMidiNote.end() && synthEngine) {
        synthEngine->noteOn(it->second, 0.8f);
    }
}

void InputMapper::keyReleased(int key) {
    auto it = keyToMidiNote.find(key);
    if (it != keyToMidiNote.end() && synthEngine) {
        synthEngine->noteOff(it->second);
    }
}
