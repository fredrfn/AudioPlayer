#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ui/main_window.hpp"
#include "sounds/players/sound_files_player.hpp"
#include "sounds/processors/amplifier.hpp"
#include <vector>
#include <QString>

class QApplication;
class App {
    QApplication& qtApplication;
    MainWindow window;
public:
    // Base
    App(QApplication& app);
    int run();
    void quit();
    MainWindow& ui() { return window; }
    // Audio
    SoundFilesPlayer audioPlayer;
    Amplifier amplifier;
};

#endif