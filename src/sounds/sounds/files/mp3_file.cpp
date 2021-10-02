#include "sounds/sounds/files/mp3_file.hpp"
#include <iostream>
#include <cstring>

MP3File::MP3File(const std::string& filePath) : 
    SoundFile(filePath), 
    decoder(OpenMP3::Decoder(openmp3)),
    itr(OpenMP3::Iterator(openmp3, nullptr, 0)),
    isDecoderRunning(false)
 {
    if (!input) { return; }
    
    parseID3V2();
    moveFileCursor(id3v2.size);

    mp3Bytes = new OpenMP3::UInt8[size() - id3v2.size]; // breaks my heart. OpenMP3 must support other iteration modes
    readBytes((int)size() - id3v2.size, (char*)mp3Bytes);
  
    itr = OpenMP3::Iterator(openmp3, mp3Bytes, (OpenMP3::UInt)size() - id3v2.size);
    float buffer[2][1152];

    if (itr.GetNext(frame)) {
		OpenMP3::UInt nsamples = decoder.ProcessFrame(frame, buffer);
        _samplingRate = frame.GetSampleRate();
        auto bitrate = frame.GetBitRate();
        if (bitrate == 0) {
            return;
        }
        _channelsCount = 2;
	    _sampleCount = _samplingRate * 8 * (size() - id3v2.size) / bitrate; // temporary estimation
        _isValid = true;
        for(int ch = 0; ch < 2; ++ch ) {
            samples[ch].reserve(_sampleCount);
            samples[ch].insert(samples[ch].end(), &buffer[ch][0], &buffer[ch][nsamples]);
		}
        decoderThread = std::thread([](MP3File* file) { file->decodeFrames(); }, this);
 	} else {
        std::cout << "MP3File: no frames could be found" << std::endl;
    }
}	

void MP3File::getSamples(SampleCount at, SoundBuffer& buffer) {
    auto maxSampleIndexRequested = at + buffer.length();
    while(isDecoderRunning && maxSampleIndexRequested >= samples[1].size()) {} // the decoder thread theoretically takes care of that

    for (SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
        for (ChannelsCount channel = 0; channel < _channelsCount; channel++) {
            buffer.write(sample, channel, samples[channel].size() > sample + at ? samples[channel].at(sample + at) : 0.0f);
        }
    }
}

void MP3File::decodeFrames() {
    isDecoderRunning = true;
    float buffer[2][1152];
    while(
        isDecoderRunning && 
        itr.GetNext(frame)
    ) {
        OpenMP3::UInt nsamples = decoder.ProcessFrame(frame, buffer);
        for(int ch = 0; ch < 2; ++ch ) {
            samples[ch].insert(samples[ch].end(), &buffer[ch][0], &buffer[ch][nsamples]);
		}
        // There is currently an issue with this method.
        // Some MP3 do have a different bitrate/sample rate for some frames
        // That requires an additional processing to fill/interpolate/remove samples
    }
    _sampleCount = samples[0].size();
    delete[] mp3Bytes;
    isDecoderRunning = false;
}

void MP3File::parseID3V2() {
    moveFileCursor(0);
    if (readByte() == 'I' && readByte() == 'D' && readByte() == '3') {
        id3v2.version = readByte();
        id3v2.revision = readByte();
        id3v2.flags = readByte();
        id3v2.size = 
            // Big endian synch-safe int 32
            readByte() * 0x200000 +   //2^21
            readByte() * 0x4000 +     //2^14
            readByte() * 0x80 +       //2^7
            readByte() + 10;
    } 
}
