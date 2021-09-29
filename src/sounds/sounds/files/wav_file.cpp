#include "sounds/sounds/files/wav_file.hpp"
#include <iostream>
#include <cstring>

// Source of the WAV spec: 
// https://www.isip.piconepress.com/projects/speech/software/tutorials/production/fundamentals/v1.0/section_02/s02_01_p05.html

WAVFile::WAVFile(const std::string& filePath) : SoundFile(filePath) {
    if (input) {
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
        isFloatFormat = waveTypeFormat == 0x03; // IEEE
        _channelsCount = readUint16(); 
        _samplingRate = readUint32();
        readUint32(); // bytes per second
        readUint16(); // block alignment
        sampleSize = readUint16() / 8;
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
        auto divider = sampleSize * _channelsCount;
        _sampleCount = divider == 0 ? 0 : readUint32() / divider;
        
        // PCM data
        samplesOffset = fileCursor();

        _isValid = true;
    }
}	

void WAVFile::getSamples(SampleCount at, SoundBuffer& buffer) {
    moveFileCursor(samplesOffset + (at * sampleSize * _channelsCount));

    for (SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
        for (ChannelsCount channel = 0; channel < _channelsCount; channel++) {
            uint8_t bytes[4];
            readBytes(sampleSize, (char*)bytes);
            if (isFloatFormat) {
                float* value = reinterpret_cast<float*>(bytes);
                buffer.write(sample, channel, *value);
            } else {
                float value = 0.0f;
                switch (sampleSize) {
                    case 1:
                        // According to the spec - these are unsigned
                        value = (float)(bytes[0]) / (float)0xFF;
                        break;
                    case 2:
                        value = (float)(*reinterpret_cast<int16_t*>(bytes)) / (float)0x8000;
                        break;
                    case 3: 
                        for (uint8_t i = 0; i < 3; i++) { bytes[3 - i] = bytes[2 - i]; }
                        bytes[0] = 0;
                        value = (float)(*reinterpret_cast<int32_t*>(bytes)) / (float)0x800000;
                        break;
                    case 4:
                        value = (float)(*reinterpret_cast<int32_t*>(bytes)) / (float)0x80000000;
                        break;
                    default:
                        value = 0.0f;
                        break;
                } 
                buffer.write(sample, channel, value);
            }
        }
    }
}