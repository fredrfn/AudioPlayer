#ifndef SOUND_PROCESSOR_DELAY_HPP
#define SOUND_PROCESSOR_DELAY_HPP

#include "sounds/sound_processor.hpp"

class VaryingAmplifier : public SoundProcessor {
protected:
    float(*gains)(double time);
    virtual void processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) override;
public:
    VaryingAmplifier(float(*gains)(double time)) : SoundProcessor(), gains(gains) {}
    float gain(double time) const { return gains != nullptr ? gains(time) : 0.0f; }
    void gainsFunction(float(*value)(double)) { gains = value; }
};

#endif