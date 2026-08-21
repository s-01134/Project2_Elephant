#pragma once

// A standard linear ADSR (Attack / Decay / Sustain / Release) envelope.
// Composed directly into SynthVoice -- each voice owns exactly one,
// created and destroyed with it.
class Envelope {
public:
    void setup(float sampleRate);
    void noteOn();
    void noteOff();

    // Advances the envelope by one sample and returns the current
    // amplitude multiplier (0..1).
    float process();

    bool isIdle() const { return stage == Stage::Idle; }

    // Public parameters -- iteration 7's GUI panel binds sliders
    // directly to these.
    float attackTime = 0.01f;   // seconds
    float decayTime = 0.15f;    // seconds
    float sustainLevel = 0.7f;  // 0..1
    float releaseTime = 0.25f;  // seconds

private:
    enum class Stage { Idle, Attack, Decay, Sustain, Release };

    Stage stage = Stage::Idle;
    float sampleRate = 44100.f;
    float currentLevel = 0.f;
    float releaseStartLevel = 0.f;
};
