#ifndef SOUNDS_PROCESSORS_FIR_FILTER_HPP
#define SOUNDS_PROCESSORS_FIR_FILTER_HPP

#include "sounds/sound_processor.hpp"
#include <vector>

#define FIR_FILTER_MAX_TAPS 1000

class FIRFilter : public SoundProcessor {
    unsigned int _order = 0;
    float _gain = 1.0f;
	std::vector<float> _taps;
    SamplingRate _lastSamplingRate = 0;
    bool hasChanged = true;
    double _lowerFrequency = 0.0;
    double _upperFrequency = 0.0;
    float oldSamples[FIR_FILTER_MAX_TAPS];
    const std::vector<float>& taps(SamplingRate samplingRate);
    std::vector<float> computetaps(SamplingRate samplingRate);
    void setFrequencies(double f1, double f2);
protected:
    virtual void processSamples(Sound* sound, SampleCount at, SoundBuffer& buffer) override;
public:
    FIRFilter(unsigned int order = 0) : SoundProcessor(), _order(order), _gain(1.0f) {}
    unsigned int order() const { return _order; }
    float gain() const { return _gain; }
    bool isLowPass() const { return _upperFrequency && !_lowerFrequency; }
    bool isHighPass() const { return !_upperFrequency && _lowerFrequency; }
    bool isBandPass() const { return _upperFrequency && _lowerFrequency; }
    bool isValid(SamplingRate rate) const;
    void order(unsigned int value) { _order = __min(FIR_FILTER_MAX_TAPS, value); hasChanged = true; }
    void gain(float value) { _gain = value; hasChanged = true; }
    void setAsLowPass(double cutFrequency) { setFrequencies(0.0, cutFrequency); }
    void setAsHighPass(double cutFrequency) { setFrequencies(cutFrequency, 0.0); }
    void setAsBandPass(double cutFrequency1, double cutFrequency2);
    float centralFrequency();
    static FIRFilter lowPass(double frequency, unsigned int order = 20);
    static FIRFilter highPass(double frequency, unsigned int order = 20);
    static FIRFilter bandPass(double lowerFrequency, double higherFrequency, unsigned int order = 20);
};

#endif
