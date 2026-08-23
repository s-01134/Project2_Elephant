#pragma once
#include <vector>

// Computes a magnitude spectrum from a block of audio samples using a
// small self-contained radix-2 FFT. Pure standard C++ -- no external
// library, no openFrameworks dependency -- so it can be (and was)
// unit-tested in isolation, the same lesson applied after the WAV
// loader in iteration 4. samples.size() is rounded down to the
// nearest power of two internally if needed.
std::vector<float> computeMagnitudeSpectrum(const std::vector<float>& samples);
