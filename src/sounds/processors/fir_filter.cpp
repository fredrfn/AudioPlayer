#include "sounds/processors/fir_filter.hpp"
#include "sounds/sound.hpp"
#include "sounds/maths/commons.hpp"

// inspired by http://www.cardinalpeak.com/blog?p=1841

const std::vector<float>& FIRFilter::taps(SamplingRate samplingRate) {
    if (_lastSamplingRate == samplingRate && !hasChanged && _taps.size() > 0) { return _taps; }
    _lastSamplingRate = samplingRate;
    _taps = computetaps(samplingRate);
    hasChanged = false;
    return _taps;
}

std::vector<float> FIRFilter::computetaps(SamplingRate samplingRate) {
    std::vector<float> result;
    result.resize(_order, 0.0f);
    memset(oldSamples, 400, 0);

    double pulseLower = 2.0 * PI * _lowerFrequency / samplingRate;
    double pulseUpper = 2.0 * PI * _upperFrequency / samplingRate;

    for(unsigned int n = 0; n < _order; n++){
        double mm = (double)n - ((double)_order - 1.0) / 2.0;
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
    if (!isValid(sound->samplingRate()) || at < _order || _order == 0) return;
    if (_gain == 0) { buffer.reset(); return; }

    std::vector<float> factorTaps = taps(sound->samplingRate());
    size_t size = factorTaps.size();

    for(ChannelsCount channel = 0; channel < sound->channelsCount(); channel++) {
        for (SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
            // TOFIX: a ring/circular buffer might be better
            for(size_t i = size - 1; i >= 1; i--){
		        oldSamples[i] = oldSamples[i-1];
            }	
            oldSamples[0] = buffer.read(sample, channel);
            float result = 0;
            for(size_t i = 0; i < size; i++) result += oldSamples[i] * factorTaps[i];
            buffer.write(sample, channel, _gain * result);
        }
    }
}

bool FIRFilter::isValid(SamplingRate rate) const {
    double nyquistFrequency = rate / 2;
    return _order > 0 && (_upperFrequency || _lowerFrequency) && nyquistFrequency > 0 &&
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

float FIRFilter::centralFrequency() {
    if  (isHighPass()) {
        return (float)_lowerFrequency;
    } else if (isLowPass()) {
        return (float)_upperFrequency;
    } else {
        return (float)((_lowerFrequency + _upperFrequency) / 2.0f);
    }
}