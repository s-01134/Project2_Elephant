#pragma once
#include <vector>
#include <utility>
#include "../Synth/SynthEngine.h"

// InteractionPad is the "interaction beyond the keyboard" piece: it
// turns mouse input into two related behaviours over the same drag
// gesture.
//
//  1) THEREMIN (live, while the mouse is held): X position maps to a
//     continuously-gliding pitch, Y position to volume -- neither
//     snaps to discrete notes, unlike keyboard playing.
//  2) GESTURE PLAYBACK (on release): the dragged path is quantized
//     onto a fixed pentatonic scale and replayed as a short melody,
//     so the shape you drew becomes a musical phrase.
//
// Runs entirely on the MAIN thread. Holds a non-owning pointer to
// SynthEngine -- ASSOCIATION, the same relationship InputMapper has.
class InteractionPad {
public:
    void setup(SynthEngine* engine, float padWidth, float padHeight);

    void mousePressed(float x, float y);
    void mouseDragged(float x, float y);
    void mouseReleased(float x, float y);

    // Advances gesture playback. Call once per frame from ofApp::update().
    void update(float deltaTimeSeconds);

    // Read-only access for drawing the pad + trail -- Visualizer/ofApp
    // only reads this, never owns or modifies it from outside.
    const std::vector<std::pair<float, float>>& getRecordedPath() const { return recordedPath; }
    bool isDraggingNow() const { return isDragging; }

private:
    SynthEngine* synthEngine = nullptr; // association, not owned
    float padWidth = 1.f;
    float padHeight = 1.f;

    bool isDragging = false;
    std::vector<std::pair<float, float>> recordedPath;

    // Gesture playback state
    std::vector<int> playbackNotes;
    size_t playbackIndex = 0;
    float playbackTimer = 0.f;
    int currentlyPlayingNote = -1;
    static constexpr float NOTE_DURATION = 0.15f; // seconds per gesture step

    float xToFrequency(float x) const; // continuous, for live theremin glide
    float yToAmplitude(float y) const; // continuous, for live theremin volume
    int xToScaleNote(float x) const;   // quantized, for gesture playback
};
