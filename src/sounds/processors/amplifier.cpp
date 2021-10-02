#include "sounds/processors/amplifier.hpp"
#include "sounds/sound.hpp"

void Amplifier::processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) {
    for (unsigned long long i = 0; i < buffer.length(); i++) { 
        buffer.write(i, buffer.read(i) * currentGain); 
    }
    
    float bufferDuration = (float)buffer.channelLength() / (float)sound->samplingRate();
    
    currentGain += fmax(1.0f, bufferDuration / _transitionTime) * (targetGain - currentGain);
}
