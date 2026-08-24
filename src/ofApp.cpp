#include "ofApp.h"

void ofApp::setup() {
    // 48000 rather than 44100: on Windows, RtAudio's default device
    // auto-selection picked a wrapped ASIO driver ("Realtek ASIO")
    // that rejected the application's requested sample rate outright,
    // regardless of which rate was asked for. Explicitly requesting a
    // DirectSound (MS_DS) output device sidesteps this: DirectSound
    // resamples internally rather than requiring an exact match with
    // whatever rate the ASIO driver's own control panel is locked to.
    // settings.sampleRate and the value passed to synthEngine.setup()
    // must always match -- a mismatch here makes every note play at
    // the wrong pitch even though audio "works".
    const float SAMPLE_RATE = 48000.f;

    ofSoundStreamSettings settings;
    settings.setOutListener(this);

    auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);
    if (!devices.empty()) {
        settings.setOutDevice(devices[0]);
    }

    settings.sampleRate = SAMPLE_RATE;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = 256;
    soundStream.setup(settings);

    synthEngine.setup(SAMPLE_RATE);
    inputMapper.setup(&synthEngine);
    interactionPad.setup(&synthEngine, PAD_WIDTH, PAD_HEIGHT);
    visualizer.setup(synthEngine.getRingBuffer());

    // Knob panel: one row, 8 knobs, laid out along the bottom of the window.
    float knobY = 545.f;
    float knobRadius = 18.f;
    float startX = 55.f;
    float spacing = 92.f;

    const ofColor ENVELOPE_COLOR(90, 160, 230);   // blue
    const ofColor EFFECTS_COLOR(240, 153, 123);   // coral

    attackKnob.setup(startX + spacing * 0, knobY, knobRadius, 0.001f, 2.f, 0.01f, "Attack", ENVELOPE_COLOR);
    decayKnob.setup(startX + spacing * 1, knobY, knobRadius, 0.001f, 2.f, 0.15f, "Decay", ENVELOPE_COLOR);
    sustainKnob.setup(startX + spacing * 2, knobY, knobRadius, 0.f, 1.f, 0.7f, "Sustain", ENVELOPE_COLOR);
    releaseKnob.setup(startX + spacing * 3, knobY, knobRadius, 0.001f, 3.f, 0.25f, "Release", ENVELOPE_COLOR);
    cutoffKnob.setup(startX + spacing * 4, knobY, knobRadius, 100.f, 12000.f, 8000.f, "Cutoff", EFFECTS_COLOR);
    delayTimeKnob.setup(startX + spacing * 5, knobY, knobRadius, 0.01f, 1.f, 0.3f, "DelayTime", EFFECTS_COLOR);
    delayFeedbackKnob.setup(startX + spacing * 6, knobY, knobRadius, 0.f, 0.9f, 0.35f, "Feedback", EFFECTS_COLOR);
    gainKnob.setup(startX + spacing * 7, knobY, knobRadius, 0.f, 1.f, 0.8f, "Gain", EFFECTS_COLOR);

    allKnobs = { &attackKnob, &decayKnob, &sustainKnob, &releaseKnob,
                 &cutoffKnob, &delayTimeKnob, &delayFeedbackKnob, &gainKnob };

    auto envelopeCallback = [this](float) { updateEnvelopeFromKnobs(); };
    attackKnob.onValueChanged = envelopeCallback;
    decayKnob.onValueChanged = envelopeCallback;
    sustainKnob.onValueChanged = envelopeCallback;
    releaseKnob.onValueChanged = envelopeCallback;

    auto effectCallback = [this](float) { onEffectOrGainKnobChanged(); };
    cutoffKnob.onValueChanged = effectCallback;
    delayTimeKnob.onValueChanged = effectCallback;
    delayFeedbackKnob.onValueChanged = effectCallback;
    gainKnob.onValueChanged = effectCallback;

    // Push each knob's initial value into the engine so the sound
    // matches what the panel displays from the very first note.
    updateEnvelopeFromKnobs();
    onEffectOrGainKnobChanged();

    ofBackground(20);
}

void ofApp::updateEnvelopeFromKnobs() {
    synthEngine.setEnvelopeParams(attackKnob.getValue(), decayKnob.getValue(),
                                   sustainKnob.getValue(), releaseKnob.getValue());
}

void ofApp::onEffectOrGainKnobChanged() {
    synthEngine.setFilterCutoff(cutoffKnob.getValue());
    synthEngine.setDelayTime(delayTimeKnob.getValue());
    synthEngine.setDelayFeedback(delayFeedbackKnob.getValue());
    synthEngine.setMasterGain(gainKnob.getValue());
}

void ofApp::update() {
    interactionPad.update(ofGetLastFrameTime());
}

void ofApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("Lower octave (C4-B4): Z S X D C V G B H N J M", 20, 20);
    ofDrawBitmapString("Upper octave (C5-B5): Q 2 W 3 E R 5 T 6 Y 7 U", 20, 38);
    ofDrawBitmapString("Voice: 1=Sine 4=Saw 8=Square 9=Noise 0=Sampler(kick.wav)", 20, 56);
    ofDrawBitmapString("Mouse pad: hold+drag for theremin (X=pitch Y=volume). Drag further + release for a gesture melody.", 20, 78);

    // Oscilloscope + spectrum
    visualizer.draw(20, 94, PAD_WIDTH, 100);

    // Interaction pad outline + gesture trail
    ofNoFill();
    ofSetColor(80);
    ofDrawRectangle(PAD_X, PAD_Y, PAD_WIDTH, PAD_HEIGHT);

    const auto& path = interactionPad.getRecordedPath();
    if (path.size() > 1) {
        ofSetColor(interactionPad.isDraggingNow() ? ofColor(240, 150, 120) : ofColor(120, 120, 120));
        ofNoFill();
        ofBeginShape();
        for (const auto& pt : path) {
            ofVertex(pt.first + PAD_X, pt.second + PAD_Y);
        }
        ofEndShape(false);
    }

    // Knob panel -- a clearly lighter background rectangle separates
    // the controls visually from the rest of the window.
    ofFill();
    ofSetColor(48, 48, 58);
    ofDrawRectangle(20, 480, 760, 120);
    ofNoFill();
    ofSetColor(100, 100, 112);
    ofDrawRectangle(20, 480, 760, 120);

    ofSetColor(90, 160, 230);
    ofDrawBitmapString("ENVELOPE", 40, 500);
    ofSetColor(240, 153, 123);
    ofDrawBitmapString("EFFECTS + GAIN", 400, 500);
    for (auto* knob : allKnobs) {
        knob->draw();
    }
}

void ofApp::audioOut(ofSoundBuffer &buffer) {
    // Runs on the AUDIO thread. Must not allocate or block.
    synthEngine.render(buffer.getBuffer().data(), buffer.getNumFrames(), buffer.getNumChannels());
}

void ofApp::keyPressed(int key) {
    inputMapper.keyPressed(key);
}

void ofApp::keyReleased(int key) {
    inputMapper.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int /*button*/) {
    // Knobs take priority: check them first so clicking a knob never
    // also triggers the pad underneath it.
    for (auto* knob : allKnobs) {
        if (knob->hitTest(x, y)) {
            activeKnob = knob;
            knob->startDrag(y);
            return;
        }
    }

    if (x >= PAD_X && x <= PAD_X + PAD_WIDTH && y >= PAD_Y && y <= PAD_Y + PAD_HEIGHT) {
        interactionPad.mousePressed(x - PAD_X, y - PAD_Y);
    }
}

void ofApp::mouseDragged(int x, int y, int /*button*/) {
    if (activeKnob) {
        activeKnob->updateDrag(y);
        return;
    }

    float px = ofClamp(static_cast<float>(x) - PAD_X, 0.f, PAD_WIDTH);
    float py = ofClamp(static_cast<float>(y) - PAD_Y, 0.f, PAD_HEIGHT);
    interactionPad.mouseDragged(px, py);
}

void ofApp::mouseReleased(int x, int y, int /*button*/) {
    if (activeKnob) {
        activeKnob->endDrag();
        activeKnob = nullptr;
        return;
    }
    interactionPad.mouseReleased(x - PAD_X, y - PAD_Y);
}
