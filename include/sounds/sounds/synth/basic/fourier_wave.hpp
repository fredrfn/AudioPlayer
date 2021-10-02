#ifndef SOUND_SYNTH_BASIC_FOURIER_WAVE_HPP
#define SOUND_SYNTH_BASIC_FOURIER_WAVE_HPP

#include "sounds/sounds/synth/basic/pitched_sound.hpp"
#include "sounds/maths/fourier.hpp"

class FourierWave : public PitchedSound {
protected:
    FourierSeriesAmplitudePhase _fourierSeries; 
public:
    FourierWave(
        FourierSeriesAmplitudePhase fourierSeries,
        SamplingRate samplingRate = 44100, 
        ChannelsCount channelsCount = 2, 
        SampleCount sampleCount = SAMPLE_COUNT_INFINITE
    ) : 
        PitchedSound((Pitch)fourierSeries.frequency(), samplingRate, channelsCount, sampleCount),
        _fourierSeries(fourierSeries)
    {}
    FourierSeriesAmplitudePhase& fourierSeries() { return _fourierSeries; }
    virtual void getSamples(SampleCount at, SoundBuffer& buffer) override;
};

#endif