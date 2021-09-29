#ifndef SOUND_BACKEND_HPP
#define SOUND_BACKEND_HPP

#include "sounds/sound_buffer.hpp"

class SoundPlayer;

typedef bool(*SoundBackendCallback)(
    SoundPlayer* player, 
    const void* output, 
    void* input,
    unsigned long frameCount
);

class SoundBackend {
protected:
    SoundBackendCallback callback = nullptr;
    SoundPlayer* player = nullptr;
public:
    void setup(SoundPlayer* p, SoundBackendCallback handler) { player = p; callback = handler; }
    virtual bool open(ChannelsCount channelsCount, SamplingRate samplingRate) = 0;
    virtual void close() = 0;
};

#endif