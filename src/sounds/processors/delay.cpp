#include "sounds/processors/delay.hpp"
#include "sounds/processors/amplifier.hpp"
#include "sounds/sound.hpp"

void Delay::processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) {
    SampleCount delayIndex = (SampleCount)time() * sound->samplingRate();
    
    if(_time == 0 || _gain == 0 || at < delayIndex) { 
        return; 
    }
    
    float* delayedBufferMemory = new float[buffer.length()];
    SoundBuffer delayedBuffer(sound->channelsCount(), buffer.channelLength(), delayedBufferMemory);
    
    sound->getSamples(at - delayIndex, delayedBuffer);

    Amplifier(1.0f - _gain).process(sound, at, buffer);
    Amplifier(_gain).process(sound, at, delayedBuffer);
    
    for (SampleCount sample = 0; sample < buffer.length(); sample++) {
        buffer.write(sample, buffer.read(sample) + delayedBuffer.read(sample));
    }

    delete[] delayedBufferMemory;
}