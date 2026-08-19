#include "ofApp.h"

void ofApp::setup() {
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

    ofBackground(20);
}

void ofApp::update() {
    // Nothing yet -- iteration 5 pulls visualization data here.
}

void ofApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("Iteration 1: press and hold A / S / D / F to play notes", 20, 30);
    ofDrawBitmapString("(4-voice sine synth, no envelope yet -- notes still cut off on release)", 20, 50);
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
