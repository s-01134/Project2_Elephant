#pragma once
#include <vector>
#include <string>

// A small, self-contained struct holding decoded PCM sample data.
// Deliberately NOT tied to any openFrameworks sound class -- this
// project hit two rounds of build errors chasing exact oF sound-API
// names/paths across environments, so loading is done with plain
// standard-library file I/O instead, for one less moving part.
struct SampleData {
    std::vector<float> samples; // interleaved, frame-major, range -1..1
    int numFrames = 0;
    int numChannels = 1;

    float getSample(int frame, int channel) const {
        if (frame < 0 || frame >= numFrames) return 0.f;
        return samples[frame * numChannels + channel];
    }
};

// Loads a 16-bit PCM .wav file (mono or stereo -- the vast majority
// of short sample snippets). Returns false on any error (missing
// file, unsupported format) rather than throwing, so a missing
// sample degrades to silence instead of crashing the app.
bool loadWavFile(const std::string& path, SampleData& outData);
