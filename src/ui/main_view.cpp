#include "ui/main_view.hpp"

#include <QLayout>
#include <QLayoutItem>
#include <QMenuBar>
#include <QSplitter>
#include "app.hpp"
#include "ui/stylesheets.hpp"

#include <vector>

void MainView::init() {
    root = new QWidget(&mainWindow);
    
    splitter = new QSplitter(Qt::Horizontal, root);
    splitter->setStyleSheet(QString::fromStdString(Stylesheets::splitter()));
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
    connect(this, SIGNAL(refresh(SoundBuffer)), this, SLOT(refreshAll(SoundBuffer)));
    connect(this, SIGNAL(fireFilesChanged()), this, SLOT(fireFilesChangedCallback()));
}

void MainWindow::refreshAll(SoundBuffer buffer) {
    view->processAudioEvent(buffer);
    view->refreshAll(); 
}

void MainWindow::fireFilesChangedCallback() {
    view->fireFilesChanged();
}

void MainView::processAudioEvent(SoundBuffer buffer){
    if (app->audioCallback != nullptr) {
        app->audioCallback(app->audioCallbackContext, buffer);
    }
}

void MainView::fireFilesChanged(){
    app->fireFilesChangedCallback();
}
