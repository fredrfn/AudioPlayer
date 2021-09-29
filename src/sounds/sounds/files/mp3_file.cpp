#include "sounds/sounds/files/mp3_file.hpp"
#include <iostream>
#include <cstring>
#include <openmp3/openmp3.h>

MP3File::MP3File(const std::string& filePath) : SoundFile(filePath) {
    if (!input) { return; }

    OpenMP3::UInt8* mp3Bytes = new OpenMP3::UInt8[size()]; 
    readBytes((int)size(), (char*)mp3Bytes);

	OpenMP3::Library openmp3;
	OpenMP3::Decoder decoder(openmp3);
    OpenMP3::Iterator itr(openmp3, mp3Bytes, (OpenMP3::UInt)size());
    OpenMP3::Frame frame;
    
    float buffer[2][1152];
	
    // TOFIX: Highly inefficient, and RAM consuming
    // A way to do this would be to keep the frame objects into a vector, and only invoke decode as wanted
    // on it in getSamples() depending on the at/length requested
    // (we still need one though to get the sampling rate)
    _samplingRate = 0;
    while (itr.GetNext(frame)) {
		OpenMP3::UInt nsamples = decoder.ProcessFrame(frame, buffer);
		for(int ch = 0; ch < 2; ++ch ) {
            samples[ch].insert(samples[ch].end(), &buffer[ch][0], &buffer[ch][nsamples]);
		}
        if (_samplingRate == 0) { _samplingRate = frame.GetSampleRate(); }
 	}
    _channelsCount = 2;
	_sampleCount = samples[0].size();
    _isValid = true;

    delete[] mp3Bytes;
}	

void MP3File::getSamples(SampleCount at, SoundBuffer& buffer) {
    for (SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
        for (ChannelsCount channel = 0; channel < _channelsCount; channel++) {
            buffer.write(sample, channel, at + sample < _sampleCount ? samples[channel][sample + at] : 0.0f);
        }
    }
}