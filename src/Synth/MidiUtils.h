#pragma once
#include <cmath>

// Shared by every pitched voice type (Sine/Saw/Square) so the
// MIDI-note-to-frequency formula exists in exactly one place instead
// of being copy-pasted into three subclasses.
inline float midiNoteToFrequency(int midiNote) {
    return 440.f * powf(2.f, (midiNote - 69) / 12.f);
}
