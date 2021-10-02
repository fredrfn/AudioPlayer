#include "sounds/processors/varying_amplifier.hpp"
#include "sounds/sound.hpp"

void VaryingAmplifier::processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) {
    if (gains == nullptr) { return; }
    float currentGain = gains((double)at * sound->samplingRate());
    for (unsigned long long i = 0; i < buffer.length(); i++) { buffer.write(i, buffer.read(i) * currentGain); }  
}