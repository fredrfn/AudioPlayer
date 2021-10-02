#include "sounds/sound_player.hpp"
#include "sounds/sound_file.hpp"
#include "sounds/sounds/files/wav_file.hpp"
#include "sounds/sound_processor.hpp"
#include "sounds/sound_buffer.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <mutex>

SoundPlayer::SoundPlayer(
    Sound& sound,
    std::vector<SoundProcessor*> processors,
    void* playCallbackContext,
    void (*playCallback)(void*, const SoundBuffer& buffer),
    void (*playEndedCallback)(void*)
) : 
    sound(sound),
    processors(processors),
    playCallbackContext(playCallbackContext), 
    playCallback(playCallback),
    playEndedCallback(playEndedCallback)  
{
    backend().setup(this, [](SoundPlayer* player, [[maybe_unused]]const void* input, void* output, unsigned long channelLength) {
        // Buffer preparation
        player->destroyMutex.lock();
        Sound& sound = player->sound;
        SampleCount sampleCount = sound.sampleCount();
        SoundBuffer buffer(sound.channelsCount(), channelLength, reinterpret_cast<float*>(output));
        // Gathering and processing audio
        buffer.reset();
        sound.getSamples(player->currentSample, buffer);
        for (auto processor : player->processors) {
            processor->process(&sound, player->currentSample, buffer);
        }
        // Broadcast notification
        if (player->playCallback != nullptr) {
            player->playCallback(player->playCallbackContext, buffer);
        }
        // Playback management
        unsigned long long nextSample = player->currentSample + channelLength;
        if (nextSample > sampleCount) { 
            player->currentSample = sampleCount;
            player->stop();
            player->destroyMutex.unlock();
            if (player->playEndedCallback != nullptr) {
                player->playEndedCallback(player->playCallbackContext);
            }
            return false;
        } else {
            player->currentSample = nextSample;
        }
        player->destroyMutex.unlock();
        return player->isPlaying();
    });
}