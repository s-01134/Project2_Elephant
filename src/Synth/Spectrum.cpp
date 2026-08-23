#include "Spectrum.h"
#include <complex>
#include <cmath>

namespace {
    using Complex = std::complex<float>;

    // M_PI is a POSIX/GNU extension, not standard C++ -- MSVC doesn't
    // define it by default (would need _USE_MATH_DEFINES before every
    // <cmath> include). Defining our own avoids relying on that.
    constexpr float PI_F = 3.14159265358979323846f;

    void fft(std::vector<Complex>& a) {
        size_t n = a.size();
        if (n <= 1) return;

        std::vector<Complex> even(n / 2), odd(n / 2);
        for (size_t i = 0; i < n / 2; ++i) {
            even[i] = a[i * 2];
            odd[i] = a[i * 2 + 1];
        }
        fft(even);
        fft(odd);

        for (size_t k = 0; k < n / 2; ++k) {
            float angle = -2.0f * PI_F * static_cast<float>(k) / static_cast<float>(n);
            Complex twiddle = std::polar(1.0f, angle) * odd[k];
            a[k] = even[k] + twiddle;
            a[k + n / 2] = even[k] - twiddle;
        }
    }
}

std::vector<float> computeMagnitudeSpectrum(const std::vector<float>& samples) {
    size_t n = samples.size();
    size_t size = 1;
    while (size * 2 <= n) size *= 2; // largest power of two <= n

    if (size < 2) return {};

    std::vector<Complex> data(size);
    for (size_t i = 0; i < size; ++i) {
        // Hann window to reduce spectral leakage from the block edges.
        float window = 0.5f - 0.5f * cosf(2.0f * PI_F * i / (size - 1));
        data[i] = Complex(samples[i] * window, 0.f);
    }

    fft(data);

    std::vector<float> magnitudes(size / 2);
    for (size_t i = 0; i < size / 2; ++i) {
        magnitudes[i] = std::abs(data[i]) / static_cast<float>(size);
    }
    return magnitudes;
}
