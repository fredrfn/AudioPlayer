#ifndef PORTAUDIO_BACKEND_HPP
#define PORTAUDIO_BACKEND_HPP

#include "sounds/sound_backend.hpp"
#include "portaudio.h"

class PortaudioBackend : public SoundBackend {
    PaStream* stream = nullptr;
public:
    PortaudioBackend();
    ~PortaudioBackend();
    virtual bool open(ChannelsCount channelsCount, SamplingRate samplingRate) override;
    virtual void close() override;   
};

#endif