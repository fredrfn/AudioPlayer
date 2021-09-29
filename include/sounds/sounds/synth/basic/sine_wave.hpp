#ifndef SOUND_SYNTH_BASIC_SINE_WAVE_HPP
#define SOUND_SYNTH_BASIC_SINE_WAVE_HPP

#include "sounds/sounds/synth/pitched_sound.hpp"
#include <cmath>

class SineWave : public PitchedSound {
protected:
    float _amplitude;
    float _phaseOffset; 
public:
    SineWave(
        Pitch pitch, 
        float amplitude = 1.0f, 
        float phaseOffset = 0.0f, 
        SamplingRate samplingRate = 44100, 
        ChannelsCount channelsCount = 2, 
        SampleCount sampleCount = SAMPLE_COUNT_INFINITE
    ) : 
        PitchedSound(pitch, samplingRate, channelsCount, sampleCount),
        _amplitude(amplitude),
        _phaseOffset(phaseOffset)
    {}
    float amplitude() { return _amplitude; }
    float phaseOffset() { return _phaseOffset; }
    virtual void getSamples(SampleCount at, SoundBuffer& buffer) override {
        if (samplingRate() == 0) { return; }
        for(SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
            float time = (_phaseOffset + at + sample) / samplingRate();
            float value = amplitude() * sinf(2 * (float)M_PI * _pitch * time);
            for(ChannelsCount channel = 0; channel < channelsCount(); channel++) {
                buffer.write(sample, channel, value);
            }
        }
    }
};

#endif