#ifndef SOUND_FILES_PLAYER_HPP
#define SOUND_FILES_PLAYER_HPP

#include "sounds/sound_player.hpp"
#include <random>
#include <map>
#include <memory>

class SoundFile;

// Class wrapping the SoundPlayer to provide easy sound files playback and enqueueing
// To support a file extension:  you need to 
// - create a ***File class, defining properly its constructor and getSamples methods 
// - in the sound files player constructor, or externally, add to the supportedFormats map the desired format
class SoundFilesPlayer {
    // Player
    std::unique_ptr<Sound> sound;
    std::unique_ptr<SoundPlayer> player;
    std::vector<SoundProcessor*> processors;
    void* playCallbackContext = nullptr;
    void (*playCallback)(void*, const SoundBuffer&) = nullptr;
    void (*playEndedCallback)(void*) = nullptr;
    // Queue
    std::vector<std::string> queue;
    bool _isLooping = false;
    bool _isShuffling = false;
    unsigned int queueIndex = 0;
    // Utilities
    std::random_device randomDevice;
    void switchFile(unsigned int index);
    Sound* buildSound(const std::string& filePath);
    void sortQueue(bool ignoreCurrent = false);
public:
    SoundFilesPlayer(
        std::vector<SoundProcessor*> processors = {},
        void* playCallbackContext = nullptr,
        void (*playCallback)(void*, const SoundBuffer&) = nullptr,
        void (*playEndedCallback)(void*) = nullptr
    );
    std::map<std::string, SoundFile*(*)(const std::string&)> supportedFiles;
    // Getters
    bool isPlaying() const { return player.get() != nullptr ? player.get()->isPlaying() : false; }
    double time() const { return player.get() != nullptr ? player.get()->time() : 0.0; }
    float progress() const { return player.get() != nullptr ? player.get()->progress() : 0.0f; }
    double duration() { return player.get() != nullptr ? player.get()->duration() : 0.0; }
    bool isEmpty() const { return queue.empty(); }
    bool isLooping() const { return _isLooping; }
    bool isShuffling() const { return _isShuffling; }
    bool hasNext() const { return !isEmpty() && (queueIndex + 1 < queue.size() || _isLooping); }
    bool hasPrevious() const { return !isEmpty() && (queueIndex > 0 || _isLooping); }
    const std::string& current() { return queue[queueIndex]; }
    // Actions
    void play() { if (player.get() != nullptr) { player.get()->play(); } }
    void pause() { if (player.get() != nullptr) { player.get()->pause(); } }
    void stop()  { if (player.get() != nullptr) { player.get()->stop(); } }
    void jumpAt(double time) { if (player.get() != nullptr) { player.get()->jumpAt(time); } }
    void next() { if (hasNext()) { switchFile(queueIndex + 1); } }
    void previous() { if (hasPrevious()) { switchFile(queueIndex - 1); } }
    void toggleShuffling() { 
        _isShuffling = !_isShuffling; 
        sortQueue(); 
    }
    void toggleLoop() { _isLooping = !_isLooping; }
    void openFile(std::string file);
    void openFiles(std::vector<std::string> files);
    void openFolder(std::string folder);
    void clear();
};

#endif 