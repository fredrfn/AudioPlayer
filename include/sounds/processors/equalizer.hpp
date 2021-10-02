#ifndef SOUNDS_PROCESSORS_EQUALIZER_HPP
#define SOUNDS_PROCESSORS_EQUALIZER_HPP

#include "sounds/processors/fir_filter.hpp"
#include "sounds/music/pitch.hpp"

class Equalizer : public SoundProcessor {
    std::vector<FIRFilter> filters;
protected:
    virtual void processSamples(Sound* sound, SampleCount at, SoundBuffer& buffer) override;   
    void createBands(std::vector<Pitch> centralPitches);
public:
    Equalizer(std::vector<std::string> centralTones);
    Equalizer(std::vector<Pitch> centralPitches = {}) : SoundProcessor() { createBands(centralPitches); }
    unsigned long long bands() { return filters.size(); }
    FIRFilter& filter(unsigned long long band) { return filters[band]; }
};

#endif
