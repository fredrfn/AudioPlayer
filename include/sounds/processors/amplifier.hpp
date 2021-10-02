#ifndef SOUND_PROCESSOR_AMPLIFIER_HPP
#define SOUND_PROCESSOR_AMPLIFIER_HPP

#include "sounds/sound_processor.hpp"

class Amplifier : public SoundProcessor {
    float currentGain = 1.0f;
    float targetGain = 1.0f;
    float _transitionTime = 0.2f; // in seconds, time needed to transition from 0.0f to 1.0f 
public:
    Amplifier(float gain = 1.0f, float speed = 0.2f) : SoundProcessor(), currentGain(gain), targetGain(gain), _transitionTime(speed) {}
    float gain() const { return targetGain; }
    void gain(float gain) { targetGain = gain; }
    float transitionTime() const { return _transitionTime; }
    void transitionTime(float transitionTime) { _transitionTime = transitionTime; }
    virtual void processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) override;
};

#endif