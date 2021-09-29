#ifndef SOUND_FILE_HPP
#define SOUND_FILE_HPP

#include <string>
#include <fstream>
#include <vector>

#include "sounds/sound.hpp"

class SoundFile : public Sound {
protected:
    std::ifstream input;
    std::string _filePath;
    unsigned long long _size;
    bool _isValid = false;
    uint8_t readByte();
    char readChar();
    void readBytes(int length, char* bytes);
    uint16_t readUint16();
    uint32_t readUint32();
    void moveFileCursor(unsigned long long pos);
    unsigned long long fileCursor();
public:
    SoundFile(const std::string& filePath);
    const std::string& filePath() const { return _filePath; }
    unsigned long long size() const { return _size; }
    bool isValid() const { return _isValid; }
};

#endif