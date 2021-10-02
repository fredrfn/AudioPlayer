#ifndef SOUND_PROCESSOR_ECHO_HPP
#define SOUND_PROCESSOR_ECHO_HPP

#include "sounds/sound_processor.hpp"

class Echo : public SoundProcessor {
    unsigned int _echoes;
    double _spacing;
    float(*gains)(unsigned int);
public:
    Echo(
        unsigned int echoes = 4,
        double spacing = 1.0, 
        float (*gains)(unsigned int) = [](unsigned int echo) { return 0.6f/(echo + 1); }
    ) : SoundProcessor(), _echoes(echoes), _spacing(spacing), gains(gains) {}
    unsigned int echoes() const { return _echoes; }
    double spacing() const { return _spacing; }
    float gain(unsigned int echo) const { return gains != nullptr ? gains(echo) : 0.0f; }
    void echoes(unsigned int value) { _echoes = value; }
    void spacing(double value) { _spacing = value; }
    void gainsFunction(float(*value)(unsigned int)) { gains = value; }
    virtual void processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) override;
};

#endif