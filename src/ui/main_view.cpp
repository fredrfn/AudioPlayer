#include "ui/main_view.hpp"

#include <QLayout>
#include <QLayoutItem>
#include <QMenuBar>
#include <QSplitter>
#include "app.hpp"

#include <vector>

void MainView::init() {
    root = new QWidget(&mainWindow);
    


    splitter = new QSplitter(root);
    splitter->addWidget(sidebar.widget(this));
    splitter->addWidget(player.widget(this));

    mainLayout = new QVBoxLayout(root);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(menu.widget(this));
    mainLayout->addWidget(splitter, 1);

    root->setLayout(mainLayout);

    mainWindow.setCentralWidget(root);
}

void MainView::refresh() {
    mainWindow.setWindowTitle(tr("AudioPlayer") + QString::fromStdString(!app->audioPlayer().isEmpty() ? " - " + app->audioPlayer().current() : ""));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(tr("AudioPlayer"));
    resize(800, 600);
    setMinimumSize(800, 600);
    connect(this, SIGNAL(refresh()), this, SLOT(refreshAll()));
}
