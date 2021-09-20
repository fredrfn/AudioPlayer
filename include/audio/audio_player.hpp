#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <filesystem>

#include "audio/audio_file.hpp"
#include "backend/portaudio_backend.hpp"

class AudioPlayer {
    PortaudioBackend _backend;
    std::random_device randomDevice;
    std::vector<std::string> selectedFiles = {};
    AudioFile* currentFile = nullptr;
    unsigned int queueIndex = 0;
    bool _isPlaying = false;
    bool _isLooping = false;
    bool _isShuffling = false;
    float targetVolume = 1.0f;
    float currentVolume = 1.0f;
    unsigned long long currentSample = 0;
    void activateFile();
    void sortFiles();
public:
    AudioPlayer();
    ~AudioPlayer();
    AudioBackend& backend() { return _backend; }
    bool isPlaying() const;
    bool isLooping() const;
    bool isShuffling() const;
    // Files management
    bool hasFile() const;
    std::string currentFilePath() const;
    void openFile(std::string file);
    void openFiles(std::vector<std::string> files);
    void openFolder(std::string folder);
    // Playback management
    double time() const;
    float progress() const;
    double duration() const;
    void play();
    void pause();
    void stop();
    void jumpAt(double time);
    // Audio files queue management
    bool hasNext() const;
    bool hasPrevious() const;
    void playFile(std::string file);
    void next();
    void previous();
    void toggleShuffling();
    void toggleLoop();
    // Audio processing
    void setVolume(float volume);
    float volume() const;
};

#endif 