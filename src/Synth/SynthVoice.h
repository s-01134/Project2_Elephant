#pragma once

// Abstract base class for anything that can produce a sample of audio.
// All concrete sound sources (oscillators now, samples and FM voices
// in later iterations) derive from this. This is the INHERITANCE
// relationship at the core of the synth architecture: the engine only
// ever talks to SynthVoice*, never to a concrete type, so new sound
// sources can be added later without touching the engine at all.
class SynthVoice {
public:
    virtual ~SynthVoice() = default;

    // Start the voice for a given MIDI note number and velocity (0-1).
    virtual void noteOn(int midiNote, float velocity) = 0;

    // Release the voice. Iteration 2 hooks this into an ADSR envelope
    // instead of cutting the sound instantly.
    virtual void noteOff() = 0;

    // Render exactly one audio sample for this voice (mono, -1..1).
    virtual float renderSample() = 0;

    // Whether the voice is currently producing sound / should stay alive.
    virtual bool isActive() const = 0;

    int getMidiNote() const { return midiNote; }

protected:
    int midiNote = -1;
    bool active = false;
};
