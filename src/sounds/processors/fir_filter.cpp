#include "sounds/processors/fir_filter.hpp"
#include "sounds/sound.hpp"
#include "sounds/maths/commons.hpp"

// inspired by http://www.cardinalpeak.com/blog?p=1841

const std::vector<float>& FIRFilter::factors(SamplingRate samplingRate) {
    if (_lastSamplingRate == samplingRate && !hasChanged && _factors.size() > 0) { return _factors; }
    _lastSamplingRate = samplingRate;
    _factors = computeFactors(samplingRate);
    return _factors;
}

std::vector<float> FIRFilter::computeFactors(SamplingRate samplingRate) {
    std::vector<float> result;
    result.resize(_order, 0.0f);

    double pulseLower = 2 * PI * _lowerFrequency / samplingRate;
    double pulseUpper = 2 * PI * _upperFrequency / samplingRate;

    for(unsigned int n = 0; n < _order; n++){
        double mm = n - (_order - 1.0) / 2.0;
        if (isLowPass()) {
            result[n] = (float)((mm == 0.0) ? pulseUpper / PI : (sin(mm * pulseUpper) / (mm * PI)));
        } else if (isHighPass()) {
            result[n] = (float)((mm == 0.0) ? (1.0 - pulseLower / PI) : (-sin(mm * pulseLower) / (mm * PI)));
        } else {
            result[n] = (float)((mm == 0.0) ? ((pulseUpper - pulseLower) / PI) : (sin(mm * pulseUpper) - sin(mm * pulseLower)) / (mm * PI));
        }
    }

    return result;
}

void FIRFilter::setFrequencies(double f1, double f2) {
    _lowerFrequency = __max(0.0, f1);
    _upperFrequency = __max(0.0, f2);
    hasChanged = true;
}

void FIRFilter::processSamples(Sound* sound, SampleCount at, SoundBuffer& buffer) {
    if (!isValid(sound->samplingRate()) || at < _order) return;
    auto& factorTaps = factors(sound->samplingRate());

    float* oldSamplesMemory = new float[buffer.length()];
    SoundBuffer oldSamplesBuffer(sound->channelsCount(), buffer.channelLength(), oldSamplesMemory);
    sound->getSamples(at - _order, oldSamplesBuffer);

    for(ChannelsCount channel = 0; sound->channelsCount(); channel++) {
        for (SampleCount sample = 0; buffer.channelLength(); sample++) {
            float result = 0;
            for(unsigned int i = 0; i < _order; i++) { 
                result += oldSamplesBuffer.read(sample, channel) * factorTaps[_order - i - 1];
            }
            buffer.write(sample, channel, result);
        }
    }

    delete[] oldSamplesMemory;
}

bool FIRFilter::isValid(SamplingRate rate) const {
    double nyquistFrequency = rate / 2;
    return _order > 0 || (_upperFrequency || _lowerFrequency) && nyquistFrequency > 0 &&
        _upperFrequency < nyquistFrequency && _lowerFrequency < nyquistFrequency;  
}

void FIRFilter::setAsBandPass(double cutFrequency1, double cutFrequency2) {
    if (cutFrequency1 && cutFrequency2 && cutFrequency2 > cutFrequency1) {
        setFrequencies(cutFrequency1, cutFrequency2);  
    } 
}

FIRFilter FIRFilter::lowPass(double frequency, unsigned int order) {
    FIRFilter filter(order); 
    filter.setAsLowPass(frequency); 
    return filter; 
}

FIRFilter FIRFilter::highPass(double frequency, unsigned int order) {
    FIRFilter filter(order); 
    filter.setAsHighPass(frequency); 
    return filter; 
}

FIRFilter FIRFilter::bandPass(double lowerFrequency, double higherFrequency, unsigned int order) {
    FIRFilter filter(order); 
    filter.setAsBandPass(lowerFrequency, higherFrequency); 
    return filter; 
}
