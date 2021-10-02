#ifndef SOUNDS_PROCESSORS_FIR_FILTER_HPP
#define SOUNDS_PROCESSORS_FIR_FILTER_HPP

#include "sounds/sound_processor.hpp"
#include <vector>

class FIRFilter : public SoundProcessor {
    unsigned int _order;
	std::vector<float> _factors;
    SamplingRate _lastSamplingRate;
    bool hasChanged;
    double _lowerFrequency;
    double _upperFrequency;
    const std::vector<float>& factors(SamplingRate samplingRate);
    std::vector<float> computeFactors(SamplingRate samplingRate);
    void setFrequencies(double f1, double f2);
protected:
    virtual void processSamples(Sound* sound, SampleCount at, SoundBuffer& buffer) override;
public:
    FIRFilter(unsigned int order = 0) : SoundProcessor(), _order(order) {}
    unsigned int order() const { return _order; }
    double gain() const { return -(double)(_order * 6); }
    bool isLowPass() const { return _upperFrequency && !_lowerFrequency; }
    bool isHighPass() const { return !_upperFrequency && _lowerFrequency; }
    bool isBandPass() const { return _upperFrequency && _lowerFrequency; }
    bool isValid(SamplingRate rate) const;
    void order(unsigned int value) { _order = value; hasChanged = true; }
    void gain(double gain) { order((unsigned int)(-gain/6.0)); }
    void setAsLowPass(double cutFrequency) { setFrequencies(0.0, cutFrequency); }
    void setAsHighPass(double cutFrequency) { setFrequencies(cutFrequency, 0.0); }
    void setAsBandPass(double cutFrequency1, double cutFrequency2);
    static FIRFilter lowPass(double frequency, unsigned int order = 0);
    static FIRFilter highPass(double frequency, unsigned int order = 0);
    static FIRFilter bandPass(double lowerFrequency, double higherFrequency, unsigned int order = 0);
};

#endif
