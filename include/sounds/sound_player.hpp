#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include "sounds/backends/portaudio_backend.hpp"
#include "sounds/sound.hpp"
#include <vector>
#include <mutex>

class SoundProcessor;
class SoundBuffer;

// Class allowing basic audio playback
class SoundPlayer {
    unsigned long long currentSample = 0;
    bool _isPlaying = false;
    bool isBusy = false;
    PortaudioBackend portaudio;
    SoundBackend& backend() { return portaudio; }
    std::vector<SoundProcessor*> processors;
    void* playCallbackContext = nullptr;
    void (*playCallback)(void*, const SoundBuffer& buffer) = nullptr;
    void (*playEndedCallback)(void*) = nullptr;
    Sound& sound;
    std::mutex destroyMutex;
public:
    SoundPlayer(
        Sound& sound,
        std::vector<SoundProcessor*> processors = {},
        void* playCallbackContext = nullptr,
        void (*playCallback)(void*, const SoundBuffer& buffer) = nullptr,
        void (*playEndedCallback)(void*) = nullptr
    );
    ~SoundPlayer() { wait(); }
    void lock() { destroyMutex.lock(); }
    void unlock() { destroyMutex.unlock(); }
    void wait() { lock(); unlock(); }
    bool isPlaying() const { return _isPlaying; }
    double time() const { return sound.samplingRate() > 0 ? currentSample / sound.samplingRate() : 0.0; }
    float progress() const { return sound.sampleCount() > 0 ? (float)currentSample / sound.sampleCount() : 0.0f; }
    double duration() const { return sound.duration(); }
    void play() { _isPlaying = true; backend().open(sound.channelsCount(), sound.samplingRate()); }
    void pause() { _isPlaying = false; }
    void stop() { _isPlaying = false; currentSample = 0; }
    void jumpAt(double time) { currentSample = duration() > 0.0 ? (SampleCount)((time / duration()) * sound.sampleCount()) : 0; }
};

#endif 