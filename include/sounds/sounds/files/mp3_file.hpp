#ifndef SOUNDS_FILES_MP3_FILE_HPP
#define SOUNDS_FILES_MP3_FILE_HPP

#include "sounds/sound_file.hpp"
#include <vector>

class MP3File : public SoundFile {
	std::vector<float> samples[2]; // breaks my heart to do that, @see TOFIX in constructor
public:
    MP3File(const std::string& filePath);
    virtual void getSamples(SampleCount at, SoundBuffer& buffer) override;
};

#endif