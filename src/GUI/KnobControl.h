#pragma once
#include "ofMain.h"
#include <functional>
#include <string>

// A minimal custom rotary knob widget. openFrameworks' built-in
// ofxGui only provides flat sliders; this draws and hit-tests a
// circular knob instead, matching the panel design planned earlier
// in the project (rotary knobs, grouped section headers) rather than
// default slider bars.
class KnobControl {
public:
    void setup(float x, float y, float radius, float minValue, float maxValue,
               float initialValue, const std::string& label,
               const ofColor& accentColor = ofColor(140, 130, 220));

    void draw();

    // Returns true if this knob's circle contains (mx,my) -- ofApp
    // uses this to decide which knob (if any) should start dragging.
    bool hitTest(float mx, float my) const;
    void startDrag(float my);
    void updateDrag(float my); // call every frame while this knob is the active drag target
    void endDrag();

    float getValue() const { return value; }

    // Fired whenever the value changes via dragging.
    std::function<void(float)> onValueChanged;

private:
    float x = 0.f, y = 0.f, radius = 20.f;
    float minValue = 0.f, maxValue = 1.f, value = 0.f;
    std::string label;
    ofColor accentColor = ofColor(140, 130, 220);
    bool dragging = false;
    float dragStartY = 0.f;
    float dragStartValue = 0.f;

    float valueToAngleDegrees() const; // maps value to -135..+135 degrees, like most hardware knobs
};
