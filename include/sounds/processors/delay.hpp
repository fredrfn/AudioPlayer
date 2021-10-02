#ifndef SOUND_PROCESSOR_DELAY_HPP
#define SOUND_PROCESSOR_DELAY_HPP

#include "sounds/sound_processor.hpp"

class Delay : public SoundProcessor {
    double _time;
    float _gain;
public:
    Delay(double time = 1.0, float gain = 0.3f) : SoundProcessor(), _time(time), _gain(gain) {}
    double time() const { return _time; }
    float gain() const { return _gain; }
    void time(double time) { _time = time; }
    void gain(float gain) { _gain = gain; }
    virtual void processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer);
};

#endif