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

    ofBackground(20);
}

void ofApp::update() {
    interactionPad.update(ofGetLastFrameTime());
}

void ofApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("Iteration 4: 5 voice types, ADSR envelope, 10-voice polyphony (2 per type)", 20, 30);
    ofDrawBitmapString("Lower octave (C4-B4): Z S X D C V G B H N J M", 20, 50);
    ofDrawBitmapString("Upper octave (C5-B5): Q 2 W 3 E R 5 T 6 Y 7 U", 20, 68);
    ofDrawBitmapString("Voice: 1=Sine 4=Saw 8=Square 9=Noise 0=Sampler(kick.wav)", 20, 90);
    ofDrawBitmapString("Mouse pad below: hold+drag for theremin (X=pitch Y=volume). Drag further + release to play a gesture melody.", 20, 112);

    // Oscilloscope + spectrum, drawn above the pad
    visualizer.draw(20, 130, PAD_WIDTH, 110);

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
            ofVertex(pt.first, pt.second);
        }
        ofEndShape(false);
    }
}

void ofApp::audioOut(ofSoundBuffer &buffer) {
    // Runs on the AUDIO thread. Must not allocate or block.
    synthEngine.render(buffer.getBuffer().data(), buffer.getNumFrames(), buffer.getNumChannels());
}

void ofApp::keyPressed(int key) {
    // Runs on the MAIN thread. ofApp no longer knows which keys mean
    // which notes -- that decision now lives entirely in InputMapper.
    inputMapper.keyPressed(key);
}

void ofApp::keyReleased(int key) {
    inputMapper.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int /*button*/) {
    if (x >= PAD_X && x <= PAD_X + PAD_WIDTH && y >= PAD_Y && y <= PAD_Y + PAD_HEIGHT) {
        interactionPad.mousePressed(x - PAD_X, y - PAD_Y);
    }
}

void ofApp::mouseDragged(int x, int y, int /*button*/) {
    // Clamp to pad bounds so dragging outside the box doesn't produce
    // out-of-range frequencies/amplitudes.
    float px = ofClamp(static_cast<float>(x) - PAD_X, 0.f, PAD_WIDTH);
    float py = ofClamp(static_cast<float>(y) - PAD_Y, 0.f, PAD_HEIGHT);
    interactionPad.mouseDragged(px, py);
}

void ofApp::mouseReleased(int x, int y, int /*button*/) {
    interactionPad.mouseReleased(x - PAD_X, y - PAD_Y);
}
