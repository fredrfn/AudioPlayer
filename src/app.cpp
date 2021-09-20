#include "app.hpp"
#include <QApplication>

int App::run() {
    window.setApp(this);
    window.refreshAll();
    window.show();
    return qtApplication.exec();
}

void App::quit() {
    QApplication::exit();
}