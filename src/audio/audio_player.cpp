#include "audio/audio_player.hpp"
#include "audio/files/wav_file.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>

#include "portaudio.h"

AudioPlayer::AudioPlayer() {
    backend().setup(this, [](AudioPlayer* player, [[maybe_unused]]const void* input, void* output, unsigned long length) {
        unsigned long bytesToRead = length * player->currentFile->channelsCount() * player->currentFile->sampleSize();
        memset(output, bytesToRead, 0);
        player->currentFile->readAudioChunkFromSample(player->currentSample, output, bytesToRead);
        unsigned long long nextSample = player->currentSample + length;
        if (nextSample > player->currentFile->samplesCount()) {
            player->currentSample = player->currentFile->samplesCount();
            if (player->hasNext()) {
                player->next();
            } else {
                player->stop();
            }
            return true;
        } else {
            player->currentSample = nextSample;
        }
        return player->isPlaying();
    });
}

AudioPlayer::~AudioPlayer() {
    backend().close();
    delete currentFile;
}

std::string AudioPlayer::currentFilePath() const {
    if (currentFile != nullptr) {
        return currentFile->filePath();
    }
    return "";
}

bool AudioPlayer::hasFile() const { 
    return !selectedFiles.empty(); 
}

bool AudioPlayer::isPlaying() const {
    return _isPlaying && currentFile != nullptr; 
}

bool AudioPlayer::isLooping() const { 
    return _isLooping; 
}

bool AudioPlayer::isShuffling() const { 
    return _isShuffling; 
}

float AudioPlayer::volume() const { 
    return targetVolume; 
}

double AudioPlayer::time() const {
    return duration() * progress();
}

float AudioPlayer::progress() const {
    if (currentFile == nullptr || currentFile->samplesCount() == 0) {
        return 0.0f;
    } 
    return (float)((double)currentSample / currentFile->samplesCount());
}

double AudioPlayer::duration() const {
    return currentFile != nullptr ? currentFile->duration() : 0.0;
}

void AudioPlayer::openFile(std::string file) {
    stop();
    selectedFiles.clear();
    selectedFiles.push_back(file);
    queueIndex = 0;
    activateFile();
}

void AudioPlayer::openFiles(std::vector<std::string> files) {
    stop();
    selectedFiles.clear();
    selectedFiles.insert(selectedFiles.end(), files.begin(), files.end());
    queueIndex = 0;
    activateFile();
}

void AudioPlayer::openFolder(std::string folder) {
    stop();
    selectedFiles.clear();
    for (auto &p : std::filesystem::recursive_directory_iterator(folder)) {
        if (p.path().extension() == ".wav") {
            selectedFiles.push_back(p.path().string());
        }
    }
    queueIndex = 0;
    activateFile();
}

void AudioPlayer::play() {
    if (currentFile != nullptr) {
        _isPlaying = backend().open(currentFile);
    } else if (hasFile()) {
        activateFile();
    }
}

void AudioPlayer::pause() { 
    _isPlaying = false; 
    backend().close();
}

void AudioPlayer::stop() { 
    currentSample = 0; 
    _isPlaying = false; 
    backend().close();
}

void AudioPlayer::jumpAt(double time) { 
    if (currentFile != nullptr) {
        currentSample = (unsigned long long)(time * currentFile->sampleRate()); 
    }
}

bool AudioPlayer::hasNext() const { 
    return hasFile() && (queueIndex + 1 < selectedFiles.size() || _isLooping); 
}

void AudioPlayer::next() {
    stop();
    if (hasNext()) { 
        currentSample = 0; 
        queueIndex = (queueIndex + 1) % selectedFiles.size(); 
        activateFile(); 
    }  
}

bool AudioPlayer::hasPrevious() const { 
    return hasFile() && (queueIndex > 0 || _isLooping); 
}

void AudioPlayer::previous() { 
    stop();
    if (hasPrevious()) { 
        currentSample = 0; 
        queueIndex = (queueIndex - 1) % selectedFiles.size(); 
        activateFile(); 
    }  
}

void AudioPlayer::toggleShuffling() { 
    _isShuffling = !_isShuffling; 
    sortFiles(); 
}

void AudioPlayer::toggleLoop() {
    _isLooping = !_isLooping; 
}

void AudioPlayer::setVolume(float volume) { 
    targetVolume = volume; 
}

void AudioPlayer::activateFile() {
    std::string& filePath = selectedFiles[queueIndex];
    std::string extension = filePath.substr(filePath.size() - 3, 3);
    for(int i = 0; i < 3; i++) { extension[i] = (char)std::tolower(extension[i]); }
    if (extension == "wav") {
        auto file = new WAVFile(filePath);
        if (!file->isValid()) {
            std::cout << "Invalid WAV file" << std::endl;
            currentFile = nullptr;
            return;
        }
        if (currentFile != nullptr) { delete currentFile; }
        currentFile = file;
    } else {
        std::cout << "Unsupported format: " << extension << std::endl;
        currentFile = nullptr;
    }   
    play();
}

void AudioPlayer::sortFiles() {
    if (!hasFile()) { return; }
    if (_isShuffling) {
        std::string tmp = selectedFiles[0];
        selectedFiles[0] = selectedFiles[queueIndex];
        selectedFiles[queueIndex] = tmp;
        std::shuffle(selectedFiles.begin() + 1, selectedFiles.end(), std::mt19937(randomDevice()));
    } else {
        sort(selectedFiles.begin(), selectedFiles.end());
        if (currentFile != nullptr) {
            auto result = std::find(selectedFiles.begin(), selectedFiles.end(), currentFile->filePath());
            queueIndex = (unsigned int)(result != selectedFiles.end() ? result - selectedFiles.begin() : 0);
        }
    }
}