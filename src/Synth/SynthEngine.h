#pragma once
#include <array>
#include "VoiceManager.h"
#include "SineVoice.h"
#include "AudioRingBuffer.h"

// SynthEngine is the audio "brain" of the app, running on the audio
// thread. It owns VoiceManager (the polyphonic note pool), a
// dedicated theremin voice for continuous mouse-driven pitch/volume
// control, and an AudioRingBuffer used to publish samples to the
// Visualizer on the main thread -- all COMPOSITION.
class SynthEngine {
public:
    void setup(float sampleRate);
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);
    void setVoiceType(VoiceManager::VoiceType type);

    // Continuous "theremin" control, driven by InteractionPad's mouse
    // handling. Bypasses the discrete MIDI-note voice pool entirely --
    // this dedicated SineVoice glides smoothly instead of snapping
    // between quantized notes.
    void theraminNoteOn(float frequencyHz, float velocity);
    void theraminSetFrequency(float frequencyHz);
    void theraminSetAmplitude(float amplitude);
    void theraminNoteOff();

    // Fills an audio buffer -- called from ofApp::audioOut every block.
    void render(float* outputBuffer, int numFrames, int numChannels);

    // Association target for Visualizer -- non-owning by design.
    const AudioRingBuffer* getRingBuffer() const { return &ringBuffer; }

private:
    VoiceManager voiceManager;
    SineVoice thereminVoice;
    AudioRingBuffer ringBuffer;

    // Fixed-size scratch space for the mixed mono signal each render()
    // call -- a member, not a local/heap allocation, so nothing is
    // allocated on the audio thread per callback. Sized generously
    // above any realistic ofSoundStream buffer size.
    std::array<float, 4096> scratchBlock{};
};
