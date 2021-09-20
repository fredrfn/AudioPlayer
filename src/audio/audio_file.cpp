#include "audio/audio_file.hpp"

AudioFile::AudioFile(std::string filePath) : _filePath(filePath) {
    input = std::ifstream(filePath, std::ifstream::binary);
    if (input) {
        input.seekg(0, std::ios::end);
        _size = input.tellg();
        input.seekg (0, std::ios::beg);
    }
};

AudioFile::~AudioFile() { if (input) { input.close(); }}

bool AudioFile::isValid() const { return _isValid; }

bool AudioFile::isSampleTypeFloat() const { return _isSampleTypeFloat; }

unsigned int AudioFile::sampleRate() const { return _sampleRate; }

unsigned int AudioFile::sampleSize() const { return _sampleSize; }

unsigned int AudioFile::channelsCount() const { return _channelsCount; }

unsigned long long AudioFile::samplesCount() const { return _samplesCount; }

const std::string& AudioFile::filePath() const { return _filePath; }

double AudioFile::duration() const { return _sampleRate == 0 ? 0 : (double)_samplesCount / (double)_sampleRate; } 

bool AudioFile::hasNextChunk() const { return !input.eof(); }

unsigned long long AudioFile::size() const { return _size; }

unsigned long long AudioFile::fileCursor() {
    return input.tellg();
}

void AudioFile::moveFileCursor(unsigned long long at) {
    input.seekg(at < _size ? at : _size, std::ios::beg);
}

uint8_t AudioFile::readByte() {
    uint8_t result;
    input.read(reinterpret_cast<char*>(&result), 1);
    return result;
}

char AudioFile::readChar() {
    return readByte();
}

void AudioFile::readBytes(int length, char* bytes) {
    input.read(bytes, length);
}

uint16_t AudioFile::readUint16() {
    uint16_t result;
    input.read(reinterpret_cast<char*>(&result), 2);
    return result;
}

uint32_t AudioFile::readUint32() {
    uint32_t result;
    input.read(reinterpret_cast<char*>(&result), 4);
    return result;
}