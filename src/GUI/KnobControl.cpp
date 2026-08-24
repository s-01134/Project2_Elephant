#include "KnobControl.h"
#include <cmath>

void KnobControl::setup(float x_, float y_, float r, float minV, float maxV,
                         float initV, const std::string& lbl, const ofColor& accent) {
    x = x_; y = y_; radius = r;
    minValue = minV; maxValue = maxV;
    value = initV;
    label = lbl;
    accentColor = accent;
}

float KnobControl::valueToAngleDegrees() const {
    float t = (value - minValue) / (maxValue - minValue);
    return -135.f + t * 270.f; // -135deg (min) to +135deg (max)
}

void KnobControl::draw() {
    ofPushStyle();

    // Knob body uses a darkened version of the group's accent color
    // (rather than plain grey) so each knob visibly belongs to its
    // color-coded group (purple = envelope, coral = effects) at a
    // glance, not just its section header.
    ofColor bodyColor(static_cast<int>(accentColor.r * 0.35f),
                       static_cast<int>(accentColor.g * 0.35f),
                       static_cast<int>(accentColor.b * 0.35f));
    ofFill();
    ofSetColor(bodyColor);
    ofDrawCircle(x, y, radius);

    ofNoFill();
    ofSetColor(accentColor);
    ofSetLineWidth(2.f);
    ofDrawCircle(x, y, radius);

    float angleRad = ofDegToRad(valueToAngleDegrees() - 90.f); // -90 so 0deg points straight up
    float indicatorX = x + cosf(angleRad) * radius * 0.7f;
    float indicatorY = y + sinf(angleRad) * radius * 0.7f;

    // White indicator normally (stands out clearly against the
    // colored body); a bright gold while actively dragging, so the
    // "currently being adjusted" state is unmistakable regardless of
    // which group's color the knob belongs to.
    ofSetColor(dragging ? ofColor(255, 215, 110) : ofColor(255));
    ofSetLineWidth(2.f);
    ofDrawLine(x, y, indicatorX, indicatorY);

    ofSetColor(accentColor);
    ofDrawBitmapString(label, x - radius, y + radius + 14);

    ofPopStyle();
}

bool KnobControl::hitTest(float mx, float my) const {
    float dx = mx - x;
    float dy = my - y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void KnobControl::startDrag(float my) {
    dragging = true;
    dragStartY = my;
    dragStartValue = value;
}

void KnobControl::updateDrag(float my) {
    if (!dragging) return;
    // Vertical drag-to-adjust, like most DAW knobs: dragging up
    // increases the value, down decreases it. Deliberately not
    // angle-based -- dragging precisely around a small circle's
    // circumference is fiddly with a mouse; a fixed pixel range
    // mapped to the full value range is easier to control.
    float deltaY = dragStartY - my;
    float range = maxValue - minValue;
    float sensitivity = range / 150.f; // 150px of vertical drag = full range
    value = ofClamp(dragStartValue + deltaY * sensitivity, minValue, maxValue);
    if (onValueChanged) onValueChanged(value);
}

void KnobControl::endDrag() {
    dragging = false;
}
