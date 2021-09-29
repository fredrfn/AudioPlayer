#ifndef SOUND_FILES_PLAYER_HPP
#define SOUND_FILES_PLAYER_HPP

#include "sounds/sounds/files/wav_file.hpp"
#include "sounds/sound_player.hpp"
#include <algorithm>
#include <filesystem>
#include <memory>
#include <random>
#include <iostream>

// Class wrapping the SoundPlayer to provide easy sound files playback and enqueueing
class SoundFilesPlayer {
    // Player
    std::unique_ptr<Sound> sound;
    std::unique_ptr<SoundPlayer> player;
    std::vector<SoundProcessor*> processors;
    void* playCallbackContext = nullptr;
    void (*playCallback)(void*) = nullptr;
    void (*playEndedCallback)(void*) = nullptr;
    // Queue
    std::vector<std::string> queue;
    bool _isLooping = false;
    bool _isShuffling = false;
    unsigned int queueIndex = 0;
    // Utilities
    std::random_device randomDevice;
    void switchFile(unsigned int index) { 
        stop();
        queueIndex = index % queue.size();
        sound.reset(buildSound(queue[queueIndex]));
        if (sound != nullptr) {
            player.reset(new SoundPlayer(*(sound.get()), processors, playCallbackContext, playCallback, playEndedCallback));
        } else {
            player.release();
            sound.release();
        }
        play();
    }
    Sound* buildSound(const std::string& filePath) {
        std::string extension = filePath.substr(filePath.size() - 3, 3);
        for(int i = 0; i < 3; i++) { extension[i] = (char)std::tolower(extension[i]); }
        std::shared_ptr<Sound> file;
        if (extension == "wav") {
            return new WAVFile(filePath);
        } else {
            std::cout << "Unsupported format: " << extension << std::endl;
            return nullptr;
        }   
    }
    void sortQueue() {
        if (isEmpty()) { return; }
        if (_isShuffling) {
            auto& tmp = queue[0];
            queue[0] = queue[queueIndex];
            queue[queueIndex] = tmp;
            queueIndex = 0;
            std::shuffle(queue.begin() + 1, queue.end(), std::mt19937(randomDevice()));
        } else {
            sort(queue.begin(), queue.end());
            auto result = std::find(queue.begin(), queue.end(), current());
            queueIndex = (unsigned int)(result != queue.end() ? result - queue.begin() : 0);
        }
    }
public:
    SoundFilesPlayer(
        std::vector<SoundProcessor*> processors = {},
        void* playCallbackContext = nullptr,
        void (*playCallback)(void*) = nullptr,
        void (*playEndedCallback)(void*) = nullptr
    ) : 
        processors(processors), 
        playCallbackContext(playCallbackContext), 
        playCallback(playCallback),
        playEndedCallback(playEndedCallback) {};
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
    void toggleShuffling() { _isShuffling = !_isShuffling; sortQueue(); }
    void toggleLoop() { _isLooping = !_isLooping; }
    void openFile(std::string file) {
        clear();
        queue.emplace_back(file);
        switchFile(0);
    }
    void openFiles(std::vector<std::string> files) {
        clear();
        queue = files;
        switchFile(0);
    }
    void openFolder(std::string folder) {
        clear();
        for (auto &p : std::filesystem::recursive_directory_iterator(folder)) {
            auto extension = p.path().extension();
            if (extension == ".wav") {
                queue.push_back(p.path().string());
            }
        }
        switchFile(0);
    }
    void clear() {
        if (player.get() != nullptr) {
            player.get()->stop();
        }
        player.release(); 
        sound.release(); 
        queueIndex = 0; 
        queue.clear();  
    }
};

#endif 