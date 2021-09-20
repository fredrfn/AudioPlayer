#ifndef AUDIO_FILES_WAV_FILE_HPP
#define AUDIO_FILES_WAV_FILE_HPP

#include "audio/audio_file.hpp"

class WAVFile : public AudioFile {
    unsigned long long samplesOffset = 0;
public:
    WAVFile(std::string filePath);
    virtual void readAudioChunkFromSample(unsigned long long sample, void* destination, unsigned int length) override;
};

#endif