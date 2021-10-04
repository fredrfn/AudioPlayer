#include "sounds/processors/equalizer.hpp"
#include "sounds/sound.hpp"

Equalizer Equalizer::create31Bands() {
    return Equalizer({
        20.0f, 25.0f, 32.0f, 40.0f, 50.0f, 63.0f, 80.0f, 100.0f,	
        125.0f,	160.0f, 200.0f, 250.0f, 315.0f, 400.0f, 500.0f,	630.0f, 
        800.0f,	1000.0f, 1250.0f, 1600.0f, 2000.0f, 2500.0f, 3150.0f, 4000.0f, 
        5000.0f, 6300.0f, 8000.0f, 10000.0f, 12500.0f, 16000.0f, 20000.0f
    });
}

Equalizer Equalizer::create15Bands() {
    return Equalizer({
        25.0f, 40.0f, 63.0f, 100.0f,	
        160.0f, 250.0f, 400.0f, 630.0f, 
        1000.0f, 1600.0f, 2500.0f, 4000.0f, 
        6300.0f, 10000.0f, 16000.0f
    });
}

Equalizer Equalizer::create10Bands() {
    return Equalizer({
        32.0f, 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f
    });
}

Equalizer Equalizer::create5Bands() {
    return Equalizer({60.0f, 250.0f, 1000.0f, 3500.0f, 10000.0f});
}

Equalizer::Equalizer(std::vector<std::string> centralTones) : SoundProcessor() { 
    std::vector<Pitch> centralPitches;
    for (auto& centralTone : centralTones) {
        centralPitches.push_back(PitchData(centralTone).frequency());
    }
    createBands(centralPitches); 
}

void Equalizer::processSamples(Sound* sound, SampleCount at, SoundBuffer& outputBuffer) {
    SoundBuffer inputBuffer(sound->channelsCount(), outputBuffer.channelLength(), inputBufferMemory);
    inputBuffer.copy(outputBuffer);
    outputBuffer.reset();
    
    for (auto& filter: filters) {
        SoundBuffer tmpBuffer(sound->channelsCount(), inputBuffer.channelLength(), tmpBufferMemory);
        tmpBuffer.copy(inputBuffer);
        filter.process(sound, at, tmpBuffer);
        for (unsigned long long i = 0; i < outputBuffer.length(); i++) {
            outputBuffer.write(i, outputBuffer.read(i) + (tmpBuffer.read(i)));
        }
    }
}    

void Equalizer::createBands(std::vector<Pitch> centralPitches) {
    if (centralPitches.size() < 3) { 
        return; 
    }
    //filters.emplace_back(FIRFilter::lowPass(centralPitches[0]));
    for (unsigned int i = 1; i < centralPitches.size() - 1; i++) {
        filters.emplace_back(FIRFilter::bandPass(
            (centralPitches[i-1] + centralPitches[i])/2.0f, 
            (centralPitches[i] + centralPitches[i+1])/2.0f
        ));
    }
    //filters.emplace_back(FIRFilter::highPass(centralPitches[centralPitches.size() - 1]));

}