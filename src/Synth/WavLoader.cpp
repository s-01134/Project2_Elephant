#include "WavLoader.h"
#include <fstream>
#include <cstring>
#include <cstdint>

namespace {
    uint32_t readU32(std::ifstream& f) {
        unsigned char b[4];
        f.read(reinterpret_cast<char*>(b), 4);
        return static_cast<uint32_t>(b[0]) | (static_cast<uint32_t>(b[1]) << 8) |
               (static_cast<uint32_t>(b[2]) << 16) | (static_cast<uint32_t>(b[3]) << 24);
    }
    uint16_t readU16(std::ifstream& f) {
        unsigned char b[2];
        f.read(reinterpret_cast<char*>(b), 2);
        return static_cast<uint16_t>(b[0]) | (static_cast<uint16_t>(b[1]) << 8);
    }
}

bool loadWavFile(const std::string& path, SampleData& outData) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    char riffTag[4];
    file.read(riffTag, 4);
    if (std::strncmp(riffTag, "RIFF", 4) != 0) return false;
    readU32(file); // overall file size, unused
    char waveTag[4];
    file.read(waveTag, 4);
    if (std::strncmp(waveTag, "WAVE", 4) != 0) return false;

    uint16_t numChannels = 1;
    uint16_t bitsPerSample = 16;
    bool haveFmt = false;
    std::vector<char> pcmData;

    // Walk the chunk list. WAV files can contain extra metadata
    // chunks (LIST, fact, etc.) before or after "data" -- unknown
    // chunks are simply skipped rather than assumed absent.
    while (file.good()) {
        char chunkId[4];
        file.read(chunkId, 4);
        if (file.gcount() < 4) break;
        uint32_t chunkSize = readU32(file);

        if (std::strncmp(chunkId, "fmt ", 4) == 0) {
            uint16_t audioFormat = readU16(file);
            numChannels = readU16(file);
            readU32(file); // sample rate, unused (SynthEngine's rate is what matters for playback)
            readU32(file); // byte rate, unused
            readU16(file); // block align, unused
            bitsPerSample = readU16(file);
            long extra = static_cast<long>(chunkSize) - 16;
            if (extra > 0) file.seekg(extra, std::ios::cur);
            haveFmt = true;
            if (audioFormat != 1) return false; // only uncompressed PCM supported
        } else if (std::strncmp(chunkId, "data", 4) == 0) {
            pcmData.resize(chunkSize);
            file.read(pcmData.data(), chunkSize);
        } else {
            file.seekg(chunkSize, std::ios::cur);
        }
        if (chunkSize % 2 == 1) file.seekg(1, std::ios::cur); // chunks are word-aligned
    }

    if (!haveFmt || pcmData.empty() || bitsPerSample != 16) return false;

    int totalSamples = static_cast<int>(pcmData.size() / 2); // 2 bytes per 16-bit sample
    outData.numChannels = numChannels;
    outData.numFrames = totalSamples / numChannels;
    outData.samples.resize(totalSamples);

    for (int i = 0; i < totalSamples; ++i) {
        int16_t raw = static_cast<int16_t>(
            (static_cast<unsigned char>(pcmData[i * 2 + 1]) << 8) |
             static_cast<unsigned char>(pcmData[i * 2]));
        outData.samples[i] = raw / 32768.f;
    }

    return true;
}
