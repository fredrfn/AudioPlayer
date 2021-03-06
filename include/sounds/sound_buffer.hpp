#ifndef SOUND_BUFFER_HPP
#define SOUND_BUFFER_HPP

#include <cmath>
#include <string>

typedef unsigned long long SampleCount;
typedef unsigned int ChannelsCount;
typedef unsigned int SamplingRate;

class SoundBuffer {
    ChannelsCount channelsCount;
    SampleCount _channelLength;
    float* buffer;
public: 
    SoundBuffer(ChannelsCount channelsCount, SampleCount channelLength, float* buffer) : 
        channelsCount(channelsCount), _channelLength(channelLength), buffer(buffer) {};
    unsigned long long length() const { return _channelLength * channelsCount; }
    SampleCount channelLength() const  { return _channelLength; }
    unsigned long long bytesLength() const { return length() * 4; }
    float read(unsigned long long index) const { return buffer[index]; }
    float read(SampleCount at, ChannelsCount channel) const { return read((at * channelsCount) + channel); }
    void read(SampleCount at, SampleCount length, float* memory) const { memcpy(memory, buffer + (at * 4 * channelsCount), length * 4); }
    void write(unsigned long long index, float value) { if(index < length()) buffer[index] = fmax(-1.0f, fmin(1.0f, value)); }
    void write(SampleCount at, ChannelsCount channel, float value) { write((at * channelsCount) + channel, value); }
    void reset() { memset((unsigned char*)buffer, 0, bytesLength()); }
    void copy(const SoundBuffer& other) { memcpy((unsigned char*)buffer, (unsigned char*)other.buffer, other.bytesLength()); }
};

#endif