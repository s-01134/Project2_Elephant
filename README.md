# Project Elephant 🐘🎹

A polyphonic software synthesizer built with **openFrameworks** and **C++**, featuring multiple voice types, real-time audio visualization, and an interactive theremin-style gesture interface.

---

## Features

###  **Polyphonic Synthesis**
- **5 Voice Types**: Sine, Saw, Square, Noise, and Sampler (plays kick.wav)
- **Voice Stealing**: Intelligent voice allocation with age-based stealing
- **ADSR Envelope**: Full Attack, Decay, Sustain, Release envelope on every voice
- **Thread-Safe**: Atomic operations protect voice state between UI and audio threads

### 🎹 **Input Methods**
- **Computer Keyboard**: Two-octave piano layout (C4-B5)
  - Lower octave: `Z S X D C V G B H N J M`
  - Upper octave: `Q 2 W 3 E R 5 T 6 Y 7 U`
- **Voice Selection**: Number keys `1`, `4`, `8`, `9`, `0`
- **Mouse Theremin**: Drag on the interaction pad for continuous pitch (X) and volume (Y) control
- **Gesture Recording**: Release mouse to play back your gesture as a pentatonic melody

### 🎛️ **Audio Effects** *(Implemented, requires Xcode build configuration)*
- **Low-Pass Filter**: Simple one-pole IIR with adjustable cutoff
- **Delay Effect**: Feedback delay with adjustable time and mix

### 📊 **Real-Time Visualization**
- **Oscilloscope**: Live waveform display
- **Spectrum Analyzer**: Frequency content visualization
- **Gesture Trail**: Visual feedback for mouse interaction

---


## Building the Project

### Prerequisites

- **openFrameworks** 0.11.x or later
- **C++17** compiler (MSVC, GCC, Clang)
- **Platform**: Windows (DirectSound), macOS, or Linux

### Xcode Setup (macOS)

1. **Clone/Download** the project
2. **Add missing source files to build target**:
   - Right-click project in Xcode navigator
   - Select "Add Files to..."
   - Add these files:
     - `EffectsChain.cpp`
     - `LowPassFilter.cpp`
     - `DelayEffect.cpp`
   - Ensure your app target is checked
3. **Build and Run** (⌘R)

### Visual Studio Setup (Windows)

1. Open the `.sln` solution file
2. Ensure all `.cpp` files are included in the project
3. Build configuration: Release or Debug
4. Run (F5)

### Sample File

Place `kick.wav` in one of these locations:
- Project root
- `data/`
- `bin/data/`
- `../data/`

If missing, SamplerVoice will play silence instead of crashing.

---

## Usage

### Playing Notes

1. **Select a voice type**: Press `1` (Sine), `4` (Saw), `8` (Square), `9` (Noise), or `0` (Sampler)
2. **Play notes**: Use keyboard piano layout (see Features section)
3. **Theremin mode**: Click and drag on the interaction pad
4. **Gesture recording**: Drag a shape, then release to hear it played back

### Keyboard Layout

```
 Q  2  W  3  E  R  5  T  6  Y  7  U     ← Upper octave (C5-B5)
 Z  S  X  D  C  V  G  B  H  N  J  M    ← Lower octave (C4-B4)
```


- **Envelope**: Attack, Decay, Sustain, Release knobs
- **Effects**: Filter Cutoff, Delay Time, Delay Feedback, Master Gain knobs

---
