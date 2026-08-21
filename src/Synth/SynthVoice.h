#pragma once
#include "Envelope.h"

// Abstract base class for anything that can produce a sample of audio.
//
// From iteration 2 onward, SynthVoice owns the entire note lifecycle
// itself, including its envelope (COMPOSITION: one Envelope per
// voice, created and destroyed with it). Concrete subclasses no
// longer implement noteOn/noteOff/isActive directly -- they only
// implement the two pitch/timbre-specific hooks below. This means
// every future voice type (Saw, Square, Noise, Sampler, ...) gets
// correct envelope behaviour for free, with no duplicated code.
class SynthVoice {
public:
    virtual ~SynthVoice() = default;

    void setup(float sampleRate);
    void noteOn(int midiNote, float velocity);
    void noteOff();
    float renderSample();

    bool isActive() const { return active; }
    int getMidiNote() const { return midiNote; }

protected:
    // Subclasses implement these instead of the old noteOn/renderSample.
    virtual void onNoteOn(int midiNote, float velocity) = 0;
    virtual float generateRawSample() = 0; // -1..1, before envelope is applied

    float sampleRate = 44100.f;

private:
    int midiNote = -1;
    bool active = false;
    Envelope envelope;
};
