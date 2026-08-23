#include "Visualizer.h"
#include "../Synth/Spectrum.h"
#include <algorithm>

void Visualizer::setup(const AudioRingBuffer* buffer) {
    ringBuffer = buffer;
}

void Visualizer::draw(float x, float y, float width, float height) {
    if (!ringBuffer) return;

    std::vector<float> snapshot = ringBuffer->getSnapshot();
    if (snapshot.empty()) return;

    // --- Oscilloscope (top half) ---
    float scopeHeight = height * 0.5f;
    ofSetColor(90, 200, 160);
    ofNoFill();
    ofBeginShape();
    for (size_t i = 0; i < snapshot.size(); ++i) {
        float px = x + (static_cast<float>(i) / snapshot.size()) * width;
        float py = y + scopeHeight * 0.5f - snapshot[i] * scopeHeight * 0.5f;
        ofVertex(px, py);
    }
    ofEndShape(false);

    // --- Spectrum bars (bottom half) ---
    std::vector<float> magnitudes = computeMagnitudeSpectrum(snapshot);
    float specY = y + scopeHeight + 10.f;
    float specHeight = height - scopeHeight - 10.f;

    ofSetColor(140, 130, 220);
    ofFill();
    int numBars = std::min<int>(64, static_cast<int>(magnitudes.size()));
    float barWidth = width / static_cast<float>(numBars);
    for (int i = 0; i < numBars; ++i) {
        float mag = ofClamp(magnitudes[i] * 8.f, 0.f, 1.f); // simple gain so quiet signals are visible
        float barHeight = mag * specHeight;
        ofDrawRectangle(x + i * barWidth, specY + specHeight - barHeight, barWidth - 1.f, barHeight);
    }
}
