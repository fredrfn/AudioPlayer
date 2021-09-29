#ifndef SOUNDS_FILES_WAV_FILE_HPP
#define SOUNDS_FILES_WAV_FILE_HPP

#include "sounds/sound_file.hpp"

class WAVFile : public SoundFile {
    unsigned long long samplesOffset = 0;
    bool isFloatFormat = 0;
    unsigned int sampleSize = 0;
public:
    WAVFile(const std::string& filePath);
    virtual void getSamples(SampleCount at, SoundBuffer& buffer) override;
};

#endif