#ifndef SOUNDS_SEQUENCE_HPP
#define SOUNDS_SEQUENCE_HPP

#include "sounds/sound.hpp"
#include <vector>

class SoundSequence : public Sound {
protected:
    std::vector<Sound*> sounds;
    void updateCharacteristics() {
        _samplingRate = sounds.empty() ? 44100 : sounds[0]->samplingRate();
        _channelsCount = sounds.empty() ? 44100 : sounds[0]->channelsCount();
        _sampleCount = 0; for (auto sound : sounds) { _sampleCount += sound->sampleCount(); }
    }
public:
    virtual ~SoundSequence() { for (auto sound : sounds) { delete sound; } }
    virtual void getSamples([[maybe_unused]]SampleCount at, [[maybe_unused]]SoundBuffer& outputBuffer) {
        float result[2] = {0.0f, 0.0f};
        SoundBuffer buffer(1, 2, result);
        for (SampleCount sample = 0; sample < outputBuffer.channelLength(); sample++) {
            SampleCount current = 0;
            for (auto sound: sounds) {
                if (at + sample < current + sound->sampleCount()) {
                    sound->getSamples(at - current + sample, buffer);
                    buffer.write(sample, 0, result[0]);
                    buffer.write(sample, 1, result[1]);
                    break;
                }
                current += current + sound->sampleCount();
            }
        }
    }
    SoundSequence& operator<<(Sound* sound) {
        sounds.push_back(sound);
        updateCharacteristics();
        return (*this);
    }
};

#endif 