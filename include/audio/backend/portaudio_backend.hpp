#ifndef PORTAUDIO_BACKEND_HPP
#define PORTAUDIO_BACKEND_HPP

#include "audio/audio_backend.hpp"
#include "portaudio.h"

class PortaudioBackend : public AudioBackend {
    PaStream* stream;
public:
    PortaudioBackend();
    ~PortaudioBackend();

    bool open(AudioFile* file) override;
    void close() override;   
};

#endif