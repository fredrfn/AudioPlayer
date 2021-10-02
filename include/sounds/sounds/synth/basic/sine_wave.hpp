#ifndef SOUND_SYNTH_BASIC_SINE_WAVE_HPP
#define SOUND_SYNTH_BASIC_SINE_WAVE_HPP

#include "sounds/sounds/synth/basic/pitched_sound.hpp"

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
    );
    float amplitude() { return _amplitude; }
    float phaseOffset() { return _phaseOffset; }
    virtual void getSamples(SampleCount at, SoundBuffer& buffer) override;
};

#endif