#ifndef SOUND_SOUND_HPP
#define SOUND_SOUND_HPP

#include <cstdint>

#define SAMPLE_COUNT_INFINITE LLONG_MAX

#include "sounds/sound_buffer.hpp"

class Sound {
protected:
    SamplingRate _samplingRate;
    SampleCount _sampleCount;
    ChannelsCount _channelsCount; 
public:
    Sound(
        SamplingRate samplingRate = 44100, 
        ChannelsCount channelsCount = 2, 
        SampleCount sampleCount = SAMPLE_COUNT_INFINITE
    ) : 
        _samplingRate(samplingRate),
        _sampleCount(sampleCount), 
        _channelsCount(channelsCount) 
    {}
    virtual ~Sound() = default;
    bool isContinuous() const { return sampleCount() == SAMPLE_COUNT_INFINITE; }
    double duration() const { return sampleCount() > 0 && !isContinuous() && samplingRate() > 0 ? (double)sampleCount() / (double)samplingRate() : 0.0; }
    SampleCount sampleCount() const { return _sampleCount; }
    SamplingRate samplingRate() const { return _samplingRate; }
    ChannelsCount channelsCount() const { return _channelsCount; }
    virtual void getSamples([[maybe_unused]]SampleCount at, [[maybe_unused]]SoundBuffer& buffer) = 0;
};

#endif 