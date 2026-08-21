#include "Envelope.h"
#include <algorithm>

void Envelope::setup(float sr) {
    sampleRate = sr;
}

void Envelope::noteOn() {
    stage = Stage::Attack;
    currentLevel = 0.f;
}

void Envelope::noteOff() {
    // Capture whatever level we were actually at -- if the note is
    // released mid-attack or mid-decay, the release fade starts from
    // there, not from the sustain level. Avoids a click on quick taps.
    if (stage != Stage::Idle) {
        releaseStartLevel = currentLevel;
        stage = Stage::Release;
    }
}

float Envelope::process() {
    switch (stage) {
        case Stage::Attack: {
            float rate = 1.f / std::max(attackTime * sampleRate, 1.f);
            currentLevel += rate;
            if (currentLevel >= 1.f) {
                currentLevel = 1.f;
                stage = Stage::Decay;
            }
            break;
        }
        case Stage::Decay: {
            float rate = (1.f - sustainLevel) / std::max(decayTime * sampleRate, 1.f);
            currentLevel -= rate;
            if (currentLevel <= sustainLevel) {
                currentLevel = sustainLevel;
                stage = Stage::Sustain;
            }
            break;
        }
        case Stage::Sustain:
            currentLevel = sustainLevel;
            break;
        case Stage::Release: {
            float rate = releaseStartLevel / std::max(releaseTime * sampleRate, 1.f);
            currentLevel -= rate;
            if (currentLevel <= 0.f) {
                currentLevel = 0.f;
                stage = Stage::Idle;
            }
            break;
        }
        case Stage::Idle:
            currentLevel = 0.f;
            break;
    }
    return currentLevel;
}
