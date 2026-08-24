#include "InteractionPad.h"
#include <cmath>
#include <algorithm>

namespace {
    // C major pentatonic across two octaves -- deliberately avoids any
    // "wrong" note, so a freehand mouse gesture always sounds musical
    // regardless of the exact path drawn.
    const int PENTATONIC_SCALE[] = {60, 62, 64, 67, 69, 72, 74, 76, 79, 81};
    const int SCALE_SIZE = 10;
}

void InteractionPad::setup(SynthEngine* engine, float w, float h) {
    synthEngine = engine;
    padWidth = w;
    padHeight = h;
}

float InteractionPad::xToFrequency(float x) const {
    float t = std::clamp(x / padWidth, 0.f, 1.f);
    // 220Hz (A3) to 880Hz (A5), exponential so it feels musically even
    // across the pad rather than bunching up at one end.
    return 220.f * powf(4.f, t);
}

float InteractionPad::yToAmplitude(float y) const {
    float t = std::clamp(y / padHeight, 0.f, 1.f);
    return std::clamp(1.f - t, 0.1f, 1.f); // top of pad = loud, bottom = quiet
}

int InteractionPad::xToScaleNote(float x) const {
    float t = std::clamp(x / padWidth, 0.f, 0.999f);
    int index = static_cast<int>(t * SCALE_SIZE);
    return PENTATONIC_SCALE[std::clamp(index, 0, SCALE_SIZE - 1)];
}

void InteractionPad::mousePressed(float x, float y) {
    if (!synthEngine) return;
    isDragging = true;
    recordedPath.clear();
    recordedPath.push_back({x, y});

    synthEngine->thereminNoteOn(xToFrequency(x), yToAmplitude(y));
}

void InteractionPad::mouseDragged(float x, float y) {
    if (!synthEngine || !isDragging) return;
    recordedPath.push_back({x, y});

    synthEngine->thereminSetFrequency(xToFrequency(x));
    synthEngine->thereminSetAmplitude(yToAmplitude(y));
}

void InteractionPad::mouseReleased(float /*x*/, float /*y*/) {
    if (!synthEngine) return;
    isDragging = false;
    synthEngine->thereminNoteOff();

    // Only treat this as a "gesture" worth replaying if the drag was
    // long enough to be an intentional shape rather than a tap.
    if (recordedPath.size() < 4) {
        recordedPath.clear();
        return;
    }

    // Sample up to 8 evenly-spaced points along the drawn path and
    // quantize each to the pentatonic scale.
    const int MAX_STEPS = 8;
    int numSteps = std::min<int>(MAX_STEPS, static_cast<int>(recordedPath.size()));
    playbackNotes.clear();
    for (int i = 0; i < numSteps; ++i) {
        size_t pathIndex = (recordedPath.size() - 1) * i / std::max(1, numSteps - 1);
        playbackNotes.push_back(xToScaleNote(recordedPath[pathIndex].first));
    }
    playbackIndex = 0;
    playbackTimer = 0.f;
}

void InteractionPad::update(float deltaTimeSeconds) {
    if (!synthEngine) return;

    if (playbackIndex >= playbackNotes.size()) {
        if (currentlyPlayingNote != -1) {
            synthEngine->noteOff(currentlyPlayingNote);
            currentlyPlayingNote = -1;
        }
        return;
    }

    playbackTimer += deltaTimeSeconds;
    if (playbackTimer >= NOTE_DURATION) {
        playbackTimer = 0.f;
        if (currentlyPlayingNote != -1) {
            synthEngine->noteOff(currentlyPlayingNote);
        }
        currentlyPlayingNote = playbackNotes[playbackIndex];
        synthEngine->noteOn(currentlyPlayingNote, 0.7f);
        ++playbackIndex;
    }
}
