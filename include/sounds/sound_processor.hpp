#ifndef SOUND_PROCESSOR_HPP
#define SOUND_PROCESSOR_HPP

#include "sounds/sound_buffer.hpp"

class Sound;
class SoundProcessor {
protected:
    bool _enabled;
    virtual void processSamples(Sound* sound, SampleCount at, SoundBuffer& buffer) = 0;
public:
    SoundProcessor() : _enabled(true) {}
    void enable() { _enabled = true; }
    void disable() { _enabled = false; }
    void toggle() { _enabled = !_enabled; }
    bool isEnabled() const { return _enabled; }
    void process(Sound* sound, SampleCount at, SoundBuffer& buffer) {
        if (_enabled) { processSamples(sound, at, buffer); }
    }
};

#endif