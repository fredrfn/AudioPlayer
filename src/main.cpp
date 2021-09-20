#include "app.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication qtApplication(argc, argv);
    return App(qtApplication).run();
}
