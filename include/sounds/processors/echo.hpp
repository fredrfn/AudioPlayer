#ifndef SOUND_PROCESSOR_ECHO_HPP
#define SOUND_PROCESSOR_ECHO_HPP

#include "sounds/sound_processor.hpp"

class Echo : public SoundProcessor {
    unsigned int _echoes;
    double _spacing;
    float _gain;
public:
    Echo(
        unsigned int echoes = 3,
        double spacing = 1.0, 
        float gain = 0.5f
    ) : SoundProcessor(), _echoes(echoes), _spacing(spacing), _gain(gain) {}
    unsigned int echoes() const { return _echoes; }
    double spacing() const { return _spacing; }
    float gain() const { return _gain; }
    void gain(float value) { _gain = value; }
    void echoes(unsigned int value) { _echoes = value; }
    void spacing(double value) { _spacing = value; }
    virtual void processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) override;
};

#endif