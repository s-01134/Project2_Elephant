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

    ofBackground(20);
}

void ofApp::update() {
    // Nothing yet -- iteration 5 pulls visualization data here.
}

void ofApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("Iteration 4: 5 voice types, ADSR envelope, 10-voice polyphony (2 per type)", 20, 30);
    ofDrawBitmapString("Lower octave (C4-B4): Z S X D C V G B H N J M", 20, 50);
    ofDrawBitmapString("Upper octave (C5-B5): Q 2 W 3 E R 5 T 6 Y 7 U", 20, 68);
    ofDrawBitmapString("Voice: 1=Sine 4=Saw 8=Square 9=Noise 0=Sampler(kick.wav)", 20, 90);
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
