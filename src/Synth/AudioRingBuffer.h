#pragma once
#include <vector>
#include <mutex>

// A small circular buffer used to publish audio data from the AUDIO
// thread (SynthEngine::render) to the MAIN thread (Visualizer),
// without ever making the audio thread wait.
//
// write() uses try_lock: if the main thread happens to be reading at
// that exact instant, the audio thread simply skips publishing that
// block rather than blocking -- an occasional dropped visualization
// frame is imperceptible, but the audio thread stalling even briefly
// causes an audible click. getSnapshot() uses a normal lock, since
// the main thread can safely afford a brief wait for a fast copy.
class AudioRingBuffer {
public:
    void setup(int capacity);

    // Called from the AUDIO thread. Never blocks.
    void write(const float* samples, int numSamples);

    // Called from the MAIN thread. Returns the buffer contents in
    // chronological order (oldest sample first).
    std::vector<float> getSnapshot() const;

private:
    mutable std::mutex mutex;
    std::vector<float> buffer;
    int writeIndex = 0;
    int capacity = 0;
};
