#include "sounds/players/sound_files_player.hpp"
#include "sounds/sounds/files/wav_file.hpp"
#include "sounds/sounds/files/mp3_file.hpp"

#include <algorithm>
#include <iostream>
#include <filesystem>

SoundFilesPlayer::SoundFilesPlayer(
    std::vector<SoundProcessor*> processors,
    void* playCallbackContext,
    void (*playCallback)(void*, const SoundBuffer& buffer),
    void (*playEndedCallback)(void*)
) : 
    processors(processors), 
    playCallbackContext(playCallbackContext), 
    playCallback(playCallback),
    playEndedCallback(playEndedCallback) {
    supportedFiles["wav"] = [](const std::string& filePath) { return (SoundFile*)(new WAVFile(filePath)); };
    supportedFiles["mp3"] = [](const std::string& filePath) { return (SoundFile*)(new MP3File(filePath)); };
};

void SoundFilesPlayer::switchFile(unsigned int index) { 
    if (queue.size() == 0) { return; }
    unsigned int newValue = index % queue.size();
    bool isGoingForward = queueIndex < newValue;
    queueIndex = newValue;
    if (player != nullptr) {
        player.get()->wait();
    }
    SoundFile* result = buildSound(queue[queueIndex]);
    if (result != nullptr) {
        player.reset(new SoundPlayer(*result, processors, playCallbackContext, playCallback, playEndedCallback));
        sound.reset(result);
        play();
    } else {
        if (isGoingForward && hasNext()) {
            next();
        } else if (!isGoingForward && hasPrevious()) {
            previous();  
        } else {
            stop();
        }
    }
}

SoundFile* SoundFilesPlayer::buildSound(const std::string& filePath) {
    std::string extension = filePath.substr(filePath.size() - 3, 3);
    for(int i = 0; i < 3; i++) { extension[i] = (char)std::tolower(extension[i]); }
    auto it = supportedFiles.find(extension);
    if(it != supportedFiles.end()) {
        SoundFile* soundFile = it->second(filePath);
        if (soundFile->isValid()) {
            return soundFile;
        } else {
            delete soundFile;
        }
    } 
    std::cout << "Unsupported file: " << filePath << std::endl;
    return nullptr;
}

void SoundFilesPlayer::sortQueue(bool ignoreCurrent) {
    if (isEmpty()) { return; }
    if (_isShuffling) {
        if (ignoreCurrent) {
            std::shuffle(queue.begin(), queue.end(), std::mt19937(randomDevice()));
        } else {
            auto& tmp = queue[0];
            queue[0] = queue[queueIndex];
            queue[queueIndex] = tmp;
            queueIndex = 0;
            std::shuffle(queue.begin() + 1, queue.end(), std::mt19937(randomDevice()));
        }
    } else {
        std::string currentFile = current();
        sort(queue.begin(), queue.end());
        if (!ignoreCurrent) {
            auto result = std::find(queue.begin(), queue.end(), currentFile);
            queueIndex = (unsigned int)(result != queue.end() ? result - queue.begin() : 0);
        }
    }
}

void SoundFilesPlayer::openFile(std::string file) {
    clear();
    queue.emplace_back(file);
    switchFile(0);
}

void SoundFilesPlayer::openFiles(std::vector<std::string> files) {
    clear();
    queue = files;
    sortQueue(true);
    switchFile(0);
}

void SoundFilesPlayer::openFolder(std::string folder) {
    clear();
    for (auto &p : std::filesystem::directory_iterator(folder)) {
        std::string extension = p.path().extension().string();
        for (auto const& [supported, _] : supportedFiles) {
            if ((extension) == "." + supported) {
                queue.push_back(p.path().string());
            }
        }
    }
    sortQueue(true);
    switchFile(0);
}

void SoundFilesPlayer::clear() {
    if (player.get() != nullptr) {
        player.get()->stop();
    }
    player.release(); 
    sound.release(); 
    queueIndex = 0; 
    queue.clear();  
}
