#include "ui/views/menu_view.hpp"

#include <QMenuBar>
#include "app.hpp"
#include <QFileDialog>

void MenuView::init() {
    menubar = new QMenuBar;
    
    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MenuView::open);

    openMultipleAction = new QAction(tr("&Open multiple"), this);
    connect(openMultipleAction, &QAction::triggered, this, &MenuView::openMultiple);

    openFolderAction = new QAction(tr("&Open folder"), this);
    connect(openFolderAction, &QAction::triggered, this, &MenuView::openFolder);

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MenuView::exit);

    playAction = new QAction(tr("&Play"), this);
    connect(playAction, &QAction::triggered, this, &MenuView::play);

    pauseAction = new QAction(tr("&Pause"), this);
    connect(pauseAction, &QAction::triggered, this, &MenuView::pause);

    stopAction = new QAction(tr("&Stop"), this);
    connect(stopAction, &QAction::triggered, this, &MenuView::stop);

    previousAction = new QAction(tr("&Previous"), this);
    connect(previousAction, &QAction::triggered, this, &MenuView::previous);

    nextAction = new QAction(tr("&Next"), this);
    connect(nextAction, &QAction::triggered, this, &MenuView::next);

    loopAction = new QAction(tr("&Loop"), this);
    connect(loopAction, &QAction::triggered, this, &MenuView::loop);

    shuffleAction = new QAction(tr("&Shuffle"), this);
    connect(nextAction, &QAction::triggered, this, &MenuView::shuffle);

    volumeUpAction = new QAction(tr("&Increase Volume"), this);
    connect(volumeUpAction, &QAction::triggered, this, &MenuView::volumeUp);

    volumeDownAction = new QAction(tr("&Decrease Volume"), this);
    connect(volumeDownAction, &QAction::triggered, this, &MenuView::volumeDown);

    muteAction = new QAction(tr("&Mute"), this);
    connect(muteAction, &QAction::triggered, this, &MenuView::mute);

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &MenuView::about);

    equalizationAction = new QAction(tr("&Equalization"), this);
    connect(equalizationAction, &QAction::triggered, this, &MenuView::equalization);

    phasingAction = new QAction(tr("&Phasing"), this);
    connect(phasingAction, &QAction::triggered, this, &MenuView::phasing);

    flangingAction = new QAction(tr("&Flanging"), this);
    connect(flangingAction, &QAction::triggered, this, &MenuView::flanging);

    panningAction = new QAction(tr("&Panning"), this);
    connect(panningAction, &QAction::triggered, this, &MenuView::panning);

    delayAction = new QAction(tr("&Delay"), this);
    connect(delayAction, &QAction::triggered, this, &MenuView::delay);

    reverbAction = new QAction(tr("&Reverb"), this);
    connect(reverbAction, &QAction::triggered, this, &MenuView::reverb);

    chorusAction = new QAction(tr("&Chorus"), this);
    connect(chorusAction, &QAction::triggered, this, &MenuView::chorus);

    tremoloAction = new QAction(tr("&Tremolo"), this);
    connect(tremoloAction, &QAction::triggered, this, &MenuView::tremolo);

    filteringAction = new QAction(tr("&Filtering"), this);
    connect(filteringAction, &QAction::triggered, this, &MenuView::filtering);

    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(openMultipleAction);
    fileMenu->addAction(openFolderAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    playbackMenu = menubar->addMenu(tr("&Playback"));
    playbackMenu->addAction(playAction);
    playbackMenu->addAction(pauseAction);
    playbackMenu->addAction(stopAction);
    playbackMenu->addSeparator();
    playbackMenu->addAction(volumeUpAction);
    playbackMenu->addAction(volumeDownAction);
    playbackMenu->addAction(shuffleAction);
    playbackMenu->addAction(loopAction);
    playbackMenu->addSeparator();
    playbackMenu->addAction(previousAction);
    playbackMenu->addAction(nextAction);

    audioMenu = menubar->addMenu(tr("&Audio"));
    audioMenu->addAction(equalizationAction);
    audioMenu->addAction(phasingAction);
    audioMenu->addAction(flangingAction);
    audioMenu->addAction(panningAction);
    audioMenu->addAction(delayAction);
    audioMenu->addAction(reverbAction);
    audioMenu->addAction(chorusAction);
    audioMenu->addAction(tremoloAction);
    audioMenu->addAction(filteringAction);

    helpMenu = menubar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);

    root = menubar;
}

void MenuView::refresh() {
    playAction->setEnabled(app->audioPlayer().hasFile() && !app->audioPlayer().isPlaying());
    pauseAction->setEnabled(app->audioPlayer().isPlaying());
    stopAction->setEnabled(app->audioPlayer().hasFile());
    nextAction->setEnabled(app->audioPlayer().hasNext());
    previousAction->setEnabled(app->audioPlayer().hasPrevious());
}

void MenuView::about() {

}

void MenuView::open() {
    QString fileName = QFileDialog::getOpenFileName(
        this->fileMenu, 
        tr("Open Audio File"),
        "/home", 
        tr("Audio Files (*.wav)")
    );
    if (fileName.isEmpty() || fileName.isNull()) { return; }
    app->audioPlayer().openFile(fileName.toStdString());
    app->ui().refreshAll();
}

void MenuView::openMultiple() {
    QStringList files = QFileDialog::getOpenFileNames(
        this->fileMenu, 
        tr("Open Audio Files"),
        "/home", 
        tr("Audio Files (*.wav)")
    );
    if(files.empty()) { return; }

    std::vector<std::string> fileNames = {};
    for(auto& file : files) { fileNames.push_back(file.toStdString()); }
    
    app->audioPlayer().openFiles(fileNames);
    app->ui().refreshAll();
}

void MenuView::openFolder() {
    QString folder = QFileDialog::getExistingDirectory(
        this->fileMenu, 
        tr("Choose directory"),
        "/home"
    );
    if(!folder.isEmpty() && !folder.isNull()){
        app->audioPlayer().openFolder(folder.toStdString());
        app->ui().refreshAll();
    }
}

void MenuView::exit() {
    app->quit();
}

void MenuView::play() {
    app->audioPlayer().play();
    app->ui().refreshAll();
}

void MenuView::pause() {
    app->audioPlayer().pause();
    app->ui().refreshAll();
}

void MenuView::stop() {
    app->audioPlayer().stop();
    app->ui().refreshAll();
}

void MenuView::next() {
    app->audioPlayer().next();
    app->ui().refreshAll();
}

void MenuView::previous() {
    app->audioPlayer().previous();
    app->ui().refreshAll();
}

void MenuView::shuffle() {
    app->audioPlayer().toggleShuffling();
    app->ui().refreshAll();
}

void MenuView::loop() {
    app->audioPlayer().toggleLoop();
    app->ui().refreshAll();
}

void MenuView::volumeUp() {
    float volume = app->audioPlayer().volume();
    app->audioPlayer().setVolume(volume + 0.1f <= 1.0f ? volume + 0.1f : 1.0f);
    app->ui().refreshAll();
}

void MenuView::volumeDown() {
    float volume = app->audioPlayer().volume();
    app->audioPlayer().setVolume(volume - 0.1f >= 0.0f ? volume - 0.1f : 0.0f);
    app->ui().refreshAll();
}

void MenuView::mute() {
    app->audioPlayer().setVolume(0.0f);
    app->ui().refreshAll();
}

void MenuView::equalization() {

}

void MenuView::phasing() {

}

void MenuView::flanging() {

}

void MenuView::panning() {

}

void MenuView::delay() {

}

void MenuView::reverb() {

}

void MenuView::chorus() {

}

void MenuView::tremolo() {

}

void MenuView::filtering() {

}