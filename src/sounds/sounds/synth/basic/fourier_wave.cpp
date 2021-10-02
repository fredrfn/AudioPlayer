 
 #include "sounds/sounds/synth/basic/fourier_wave.hpp"
 #include "sounds/maths/commons.hpp"

void FourierWave::getSamples(SampleCount at, SoundBuffer& buffer) {
    if (samplingRate() == 0) { return; }

    for(SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
        float value = (float)_fourierSeries.evaluate((double)(at + sample)/(double)samplingRate());
        for(ChannelsCount channel = 0; channel < channelsCount(); channel++) {
            buffer.write(sample, channel, value);
        }
    }
}