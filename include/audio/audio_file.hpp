#ifndef AUDIO_FILE_HPP
#define AUDIO_FILE_HPP

#include <string>
#include <fstream>
#include <vector>

#define CHUNK_SIZE 1024

class AudioFile {
protected:
    std::ifstream input;
    std::string _filePath;
    // Format specifications
    unsigned int _channelsCount = 0;
    unsigned int _sampleRate = 0;
    unsigned int _sampleSize = 0; // in bytes
    unsigned long long _samplesCount = 0;
    unsigned long long _size = 0; // in bytes
    bool _isSampleTypeFloat = false;
    bool _isValid = false;
    // File utility functions
    uint8_t readByte();
    char readChar();
    void readBytes(int length, char* bytes);
    uint16_t readUint16();
    uint32_t readUint32();
    void moveFileCursor(unsigned long long pos);
    unsigned long long fileCursor();
public:
    AudioFile(std::string filePath);
    virtual ~AudioFile();
    bool isSampleTypeFloat() const;
    unsigned int sampleRate() const;
    unsigned int sampleSize() const;
    unsigned int channelsCount() const;
    unsigned long long samplesCount() const;
    const std::string& filePath() const;
    double duration() const;
    bool isValid() const;
    bool hasNextChunk() const;
    unsigned long long size() const;
    virtual void readAudioChunkFromSample([[maybe_unused]] unsigned long long sample, [[maybe_unused]] void* destination, [[maybe_unused]] unsigned int length) {}
};

#endif