#include "app.hpp"
#include <QApplication>

App::App(QApplication& app) : 
    qtApplication(app),
    audioPlayer(
        { &amplifier }, 
        this, 
        [](void* context) { emit (reinterpret_cast<App*>(context))->window.refresh(); },
        [](void* context) { emit (reinterpret_cast<App*>(context))->audioPlayer.next(); }
    ) 
{}

int App::run() {
    window.setApp(this);
    window.refreshAll();
    window.show();
    return qtApplication.exec();
}

void App::quit() {
    QApplication::exit();
}