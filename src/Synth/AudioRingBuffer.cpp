#include "AudioRingBuffer.h"

void AudioRingBuffer::setup(int cap) {
    capacity = cap;
    buffer.assign(capacity, 0.f);
    writeIndex = 0;
}

void AudioRingBuffer::write(const float* samples, int numSamples) {
    std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);
    if (!lock.owns_lock()) {
        return; // main thread is mid-read; skip this block rather than wait
    }
    for (int i = 0; i < numSamples; ++i) {
        buffer[writeIndex] = samples[i];
        writeIndex = (writeIndex + 1) % capacity;
    }
}

std::vector<float> AudioRingBuffer::getSnapshot() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<float> result(capacity);
    for (int i = 0; i < capacity; ++i) {
        result[i] = buffer[(writeIndex + i) % capacity];
    }
    return result;
}
