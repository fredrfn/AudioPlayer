#ifndef SOUNDS_FILES_MP3_FILE_HPP
#define SOUNDS_FILES_MP3_FILE_HPP

#include "sounds/sound_file.hpp"
#include <vector>
#include <openmp3/openmp3.h>
#include <thread>

// https://id3.org/id3v2.3.0#ID3v2_header
struct MP3ID3V2 {
    uint8_t version = 3; // usually 3
    uint8_t revision = 0; // usually 0
    uint8_t flags = 0; // usually none
    uint32_t size = 0; // 0 = no id3
    bool hasUnsynchronizationFlag() { return flags & 0b10000000; }
    bool hasExtendedHeaderFlag() { return flags & 0b01000000; }
    bool hasExperimentalIndicator() { return flags & 0b00100000; }
};

class MP3File : public SoundFile {
    MP3ID3V2 id3v2;
	std::vector<float> samples[2];
    OpenMP3::UInt8* mp3Bytes;
    OpenMP3::Library openmp3;
	OpenMP3::Decoder decoder;
    OpenMP3::Iterator itr;
    OpenMP3::Frame frame;
    bool isDecoderRunning = false;
    std::thread decoderThread;
public:
    MP3File(const std::string& filePath);
    ~MP3File() { 
        isDecoderRunning = false;
        if (decoderThread.joinable()) {
            decoderThread.join();
        }
    }
    virtual void getSamples(SampleCount at, SoundBuffer& buffer) override;
    void stopDecoder() { isDecoderRunning = false; }
    void decodeFrames();
    void parseID3V2();
};

#endif