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
    Delay _delay = {0, 0};
    Echo _echo;
    Equalizer _equalizer;
public:
    Amplifier& amplifier() { return _amplifier; }
    Delay& delay() { return _delay; }
    Echo& echo() { return _echo; }
    Equalizer& equalizer() { return _equalizer; }
    std::vector<SoundProcessor*> all() { return { &_equalizer, &_echo, &_delay, &_amplifier }; }
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
    App(QApplication& app);
    int run();
    void quit();
    MainView& ui() { return _ui; }
    AudioProcessors& processors() { return _processors; }
    SoundFilesPlayer& audioPlayer() { return _audioPlayer; }
};

#endif