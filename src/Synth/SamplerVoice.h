#pragma once
#include "SynthVoice.h"
#include "WavLoader.h"

// Plays back a pre-recorded audio snippet instead of generating a
// waveform. The sample data itself is NOT owned here -- it is loaded
// once by VoiceManager and shared, by pointer, across every
// SamplerVoice instance. This is an ASSOCIATION relationship ("uses,
// does not own") -- the same pattern InputMapper already uses for its
// pointer to SynthEngine.
class SamplerVoice : public SynthVoice {
public:
    void setSampleData(const SampleData* sample) { sampleData = sample; }

protected:
    void onNoteOn(int midiNote, float velocity) override;
    float generateRawSample() override;

private:
    const SampleData* sampleData = nullptr; // association, not owned
    double readPosition = 0.0; // fractional index into the sample, for pitch shifting
    double playbackRate = 1.0; // 1.0 = original speed/pitch
    float amplitude = 0.f;

    // The MIDI note at which the sample plays at its original,
    // recorded speed/pitch. Notes above/below this play faster/slower
    // (and therefore higher/lower) -- a simple, classic sampler
    // technique. Pitch and duration change together; this project
    // does not implement independent time-stretching.
    static constexpr int BASE_MIDI_NOTE = 60; // C4
};
