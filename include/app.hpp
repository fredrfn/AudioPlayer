#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <vector>
#include <QString>
#include "ui/main_view.hpp"
#include "sounds/players/sound_files_player.hpp"
#include "sounds/processors/amplifier.hpp"
#include "sounds/processors/delay.hpp"
#include "sounds/processors/echo.hpp"
#include "sounds/processors/equalizer.hpp"

class AudioProcessors {
    Amplifier _amplifier;
    Echo _echo;
    Equalizer _equalizer;
public:
    AudioProcessors() : _equalizer(Equalizer::create15Bands()) {}
    Amplifier& amplifier() { return _amplifier; }
    Echo& echo() { return _echo; }
    Equalizer& equalizer() { return _equalizer; }
    std::vector<SoundProcessor*> all() { return { &_equalizer, &_echo, &_amplifier }; }
    void enableAll() { for(auto processor: all()) { processor->enable(); }}
    void disableAll() { for (auto processor: all()) { processor->disable(); }}
};

class QApplication;
class App {
    QApplication& qtApplication;
    MainView _ui;
    SoundFilesPlayer _audioPlayer;
    AudioProcessors _processors;
public:
    void* audioCallbackContext = nullptr;
    void(*audioCallback)(void*, const SoundBuffer& buffer) = nullptr;
    void* filesChangedCallbackContext = nullptr;
    void(*filesChangedCallback)(void*) = nullptr;
    App(QApplication& app);
    int run();
    void quit();
    MainView& ui() { return _ui; }
    AudioProcessors& processors() { return _processors; }
    SoundFilesPlayer& audioPlayer() { return _audioPlayer; }
    void fireFilesChangedCallback() {
        if (filesChangedCallback != nullptr) {
            filesChangedCallback(filesChangedCallbackContext);
        }
    }
};

#endif