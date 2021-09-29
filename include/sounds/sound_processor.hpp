#ifndef SOUND_PROCESSOR_HPP
#define SOUND_PROCESSOR_HPP

#include "sounds/sound_buffer.hpp"

class Sound;
class SoundProcessor {
public:
    virtual void process(Sound* sound, SampleCount at, SoundBuffer& buffer) = 0;
};

#endif