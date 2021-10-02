 
 #include "sounds/sounds/synth/basic/sine_wave.hpp"
 #include "sounds/maths/commons.hpp"

 SineWave::SineWave(
    Pitch pitch, 
    float amplitude, 
    float phaseOffset, 
    SamplingRate samplingRate, 
    ChannelsCount channelsCount, 
    SampleCount sampleCount
) : 
    PitchedSound(pitch, samplingRate, channelsCount, sampleCount),
    _amplitude(amplitude),
    _phaseOffset(phaseOffset)
{}

void SineWave::getSamples(SampleCount at, SoundBuffer& buffer) {
    if (samplingRate() == 0) { return; }
    for(SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
        float time = (_phaseOffset + at + sample) / samplingRate();
        float value = amplitude() * sinf(2 * (float)PI * _pitch * time);
        for(ChannelsCount channel = 0; channel < channelsCount(); channel++) {
            buffer.write(sample, channel, value);
        }
    }
}