#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ui/main_window.hpp"
#include "audio/audio_player.hpp"
#include <vector>
#include <QString>

class QApplication;
class App {
    QApplication& qtApplication;
    MainWindow window;
    AudioPlayer _audioPlayer;
public:
    App(QApplication& app) : qtApplication(app) {}
    AudioPlayer& audioPlayer() { return _audioPlayer; }
    MainWindow& ui() { return window; }
    int run();
    void quit();
};

#endif