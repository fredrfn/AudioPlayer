#include "app.hpp"
#include <QApplication>

App::App(QApplication& app) : 
    qtApplication(app),
    _audioPlayer(
        _processors.all(), 
        this, 
        [](void* context, [[maybe_unused]]const SoundBuffer& buffer) { 
            auto a = reinterpret_cast<App*>(context);
            emit a->ui().mainWindow.refresh(buffer);
        },
        [](void* context) { emit (reinterpret_cast<App*>(context))->audioPlayer().next(); }
    ),
    audioCallback(nullptr),
    audioCallbackContext(nullptr)
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