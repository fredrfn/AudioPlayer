#include "sounds/sound_file.hpp"
#include <iostream>

SoundFile::SoundFile(const std::string& filePath) : _filePath(filePath) {
    input = std::ifstream(_filePath, std::ifstream::binary);
    if (input) {
        input.seekg(0, std::ios::end);
        _size = input.tellg();
        input.seekg(0, std::ios::beg);
    } else {
        std::cout << "File could not be open: " << _filePath << std::endl;
    }
}

unsigned long long SoundFile::fileCursor() {
    return input.tellg();
}

void SoundFile::moveFileCursor(unsigned long long at) {
    input.seekg(at < _size ? at : _size, std::ios::beg);
}

uint8_t SoundFile::readByte() {
    uint8_t result;
    input.read(reinterpret_cast<char*>(&result), 1);
    return result;
}

char SoundFile::readChar() {
    return readByte();
}

void SoundFile::readBytes(int length, char* bytes) {
    input.read(bytes, length);
}

uint16_t SoundFile::readUint16() {
    uint16_t result;
    input.read(reinterpret_cast<char*>(&result), 2);
    return result;
}

uint32_t SoundFile::readUint32() {
    uint32_t result;
    input.read(reinterpret_cast<char*>(&result), 4);
    return result;
}