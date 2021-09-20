#include "audio/files/wav_file.hpp"
#include <iostream>

// Source of the WAV spec: 
// https://www.isip.piconepress.com/projects/speech/software/tutorials/production/fundamentals/v1.0/section_02/s02_01_p05.html

WAVFile::WAVFile(std::string filePath) : AudioFile(filePath) {
    if (!input) {
        std::cout << "WAVFile(): file not found" << std::endl;
        return;
    }
    // Headers chunk
    for(const char& c : std::string("RIFF")) { if (c != readChar()) { 
        std::cout << c << std::endl;
        std::cout << "WAVFile(): RIFF not found" << std::endl;
        return; 
    } }
	readUint32(); // wave chunk size
    for(const char& c : std::string("WAVE")) { if (c != readChar()) { 
        std::cout << "WAVFile(): WAVE not found" << std::endl;
        return; 
    } }
    
    // Format details chunk
    for(const char& c : std::string("fmt ")) {
        if (c != readByte()) {
            std::cout << "WAVFile(): Fmt chunk not found" << std::endl; 
            return; 
        } 
    }
    uint32_t fmtSectionSize = readUint32();
    unsigned long long fmtSectionStart = fileCursor();
    if (fmtSectionSize < 16) { 
        std::cout << "WAVFile(): Fmt chunk incomplete" << std::endl; 
        return; 
    }

    // Format info
    uint16_t waveTypeFormat = readUint16();
    _isSampleTypeFloat = waveTypeFormat == 0x03; // IEEE
    _channelsCount = readUint16(); 
    _sampleRate = readUint32();
    readUint32(); // bytes per second
    readUint16(); // block alignment
    _sampleSize = readUint16() / 8;
    
    moveFileCursor(fmtSectionStart + fmtSectionSize);

    // Any other chunk
    int chunksNumber = 0;
    while(chunksNumber < 100 || input.eof()) {
        char chunk[5];
        chunk[4] = '\0';
        readBytes(4, chunk);
        if (strcmp("data", chunk) == 0) {
            break;
        }
        uint32_t chunkSize = readUint32();
        moveFileCursor(fileCursor() + chunkSize);
        chunksNumber++;
    }
    if (chunksNumber >= 100) {
        std::cout << "WAVFile(): unable to find data chunk" << std::endl;
        return;
    }
    
    // Data chunk
    auto divider = _sampleSize * _channelsCount;
    _samplesCount = divider == 0 ? 0 : readUint32() / divider;
    
    // PCM data
    samplesOffset = fileCursor();

    _isValid = true;
}	

void WAVFile::readAudioChunkFromSample(unsigned long long sample, void* destination, unsigned int length) {
    moveFileCursor(samplesOffset + (sample * _sampleSize * _channelsCount));
    memset(destination, 0, length);
    readBytes(length, (char*)destination);
}