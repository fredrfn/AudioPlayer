#include "sounds/processors/equalizer.hpp"

Equalizer::Equalizer(std::vector<std::string> centralTones) : SoundProcessor() { 
    std::vector<Pitch> centralPitches;
    for (auto& centralTone : centralTones) {
        centralPitches.push_back(PitchData(centralTone).frequency());
    }
    createBands(centralPitches); 
}

void Equalizer::processSamples(Sound* sound, SampleCount at, SoundBuffer& buffer) {
    for (auto& filter: filters) {
        filter.process(sound, at, buffer);
    }
}    

void Equalizer::createBands(std::vector<Pitch> centralPitches) {
    if (centralPitches.size() < 3) { 
        return; 
    }
    filters.push_back(FIRFilter::lowPass(centralPitches[0]));
    for (unsigned int i = 1; i < centralPitches.size() - 1; i++) {
        filters.push_back(FIRFilter::bandPass(centralPitches[i], centralPitches[i + 1]));
    }
    filters.push_back(FIRFilter::highPass(centralPitches[centralPitches.size() - 1]));
}