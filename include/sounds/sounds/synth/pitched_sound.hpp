#ifndef SOUNDS_PITCHED_SOUND_HPP
#define SOUNDS_PITCHED_SOUND_HPP

#include "sounds/music/pitch.hpp"
#include "sounds/sound.hpp"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

class PitchedSound : public Sound {
protected:
    Pitch _pitch;
public:
    PitchedSound(
        Pitch pitch = 440,
        SamplingRate samplingRate = 44100, 
        ChannelsCount channelsCount = 2, 
        SampleCount sampleCount = SAMPLE_COUNT_INFINITE
    ) : Sound(samplingRate, channelsCount, sampleCount), _pitch(pitch) {};
    Pitch pitch() { return _pitch; }
};

#endif 