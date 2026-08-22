#include "InputMapper.h"

void InputMapper::setup(SynthEngine* engine) {
    synthEngine = engine;

    // Classic "typing keyboard as piano" layout: bottom row is one
    // octave's white+black keys, the row above continues chromatically.
    //   Lower octave (C4-B4):  Z S X D C V G B H N J M
    //   Upper octave (C5-B5):  Q 2 W 3 E R 5 T 6 Y 7 U
    keyToMidiNote = {
        {'z', 60}, {'s', 61}, {'x', 62}, {'d', 63}, {'c', 64},
        {'v', 65}, {'g', 66}, {'b', 67}, {'h', 68}, {'n', 69},
        {'j', 70}, {'m', 71},
        {'q', 72}, {'2', 73}, {'w', 74}, {'3', 75}, {'e', 76},
        {'r', 77}, {'5', 78}, {'t', 79}, {'6', 80}, {'y', 81},
        {'7', 82}, {'u', 83},
    };

    // Deliberately avoids digits already used for sharps above (2,3,5,6,7).
    voiceTypeKeyMap = {
        {'1', VoiceManager::VoiceType::Sine},
        {'4', VoiceManager::VoiceType::Saw},
        {'8', VoiceManager::VoiceType::Square},
        {'9', VoiceManager::VoiceType::Noise},
        {'0', VoiceManager::VoiceType::Sampler},
    };
}

void InputMapper::keyPressed(int key) {
    auto vt = voiceTypeKeyMap.find(key);
    if (vt != voiceTypeKeyMap.end() && synthEngine) {
        synthEngine->setVoiceType(vt->second);
        return;
    }

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
