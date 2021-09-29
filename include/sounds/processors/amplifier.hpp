#ifndef SOUND_PROCESSOR_AMPLIFIER_HPP
#define SOUND_PROCESSOR_AMPLIFIER_HPP

#include "sounds/sound_processor.hpp"
#include "sounds/sound.hpp"

class Amplifier : public SoundProcessor {
    float currentGain = 1.0f;
    float targetGain = 1.0f;
    float _transitionTime = 0.2f; // in seconds, time needed to transition from 0.0f to 1.0f 
public:
    Amplifier(float speed = 0.2f) : _transitionTime(speed) {}
    float gain() const { return targetGain; }
    void gain(float gain) { targetGain = gain; }
    float transitionTime() const { return _transitionTime; }
    void transitionTime(float transitionTime) { _transitionTime = transitionTime; }
    virtual void process(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) override {
        for (unsigned long long i = 0; i < buffer.length(); i++) { buffer.write(i, buffer.read(i) * currentGain); }
        float bufferDuration = (float)buffer.channelLength() / (float)sound->samplingRate();
        currentGain += fmax(1.0f, bufferDuration / _transitionTime) * (targetGain - currentGain);
    }
};

#endif