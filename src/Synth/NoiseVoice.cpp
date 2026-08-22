#include "NoiseVoice.h"
#include <cstdlib>

void NoiseVoice::onNoteOn(int /*midiNote*/, float velocity) {
    amplitude = velocity;
    // midiNote is intentionally unused -- noise has no pitch.
}

float NoiseVoice::generateRawSample() {
    float random = (static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f;
    return random * amplitude;
}
