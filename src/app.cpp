#include "app.hpp"
#include <QApplication>

App::App(QApplication& app) : 
    qtApplication(app),
    _audioPlayer(
        _processors.all(), 
        this, 
        [](void* context, [[maybe_unused]]const SoundBuffer& buffer) { 
            emit (reinterpret_cast<App*>(context))->ui().mainWindow.refresh();
        },
        [](void* context) { emit (reinterpret_cast<App*>(context))->audioPlayer().next(); }
    ) 
{
    processors().disableAll();
    processors().amplifier().enable();
}

int App::run() {
    ui().setApp(this);
    ui().refreshAll();
    ui().mainWindow.show();
    return qtApplication.exec();
}

void App::quit() {
    QApplication::exit();
}