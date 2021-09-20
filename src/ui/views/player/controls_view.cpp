#include "ui/views/player/controls_view.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QSlider>
#include <QLabel>
#include "app.hpp"
#include <cmath>

void ControlsView::init() {
    root = new QWidget;
    
    playerLayout = new QHBoxLayout;

    currentTimeLabel = new QLabel("--:--");
    playerLayout->addWidget(currentTimeLabel);

    playerSlider = new QSlider(Qt::Horizontal);
    playerSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            border: 1px solid #cccccc;
            height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d4d4d4, stop:1 #f8f8f8);
        }
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);
            border: 1px solid #cccccc;
            width: 12px;
        }
    )");
    playerSlider->setMinimum(0);
    playerSlider->setMaximum(1000);
    connect(playerSlider, &QSlider::valueChanged, this, &ControlsView::jumpAt);

    playerLayout->addWidget(playerSlider);

    remainingTimeLabel = new QLabel("--:--");
    playerLayout->addWidget(remainingTimeLabel);

    buttonsLayout = new QHBoxLayout;

    playButton = setupControlButton("assets/icons/play3.png");
    connect(playButton, &QPushButton::clicked, this, &ControlsView::play);

    pauseButton = setupControlButton("assets/icons/pause2.png");
    connect(pauseButton, &QPushButton::clicked, this, &ControlsView::pause);

    stopButton = setupControlButton("assets/icons/stop2.png");
    connect(stopButton, &QPushButton::clicked, this, &ControlsView::stop);

    previousButton = setupControlButton("assets/icons/previous2.png");
    connect(previousButton, &QPushButton::clicked, this, &ControlsView::previous);

    nextButton = setupControlButton("assets/icons/next2.png");
    connect(nextButton, &QPushButton::clicked, this, &ControlsView::next);

    shuffleButton = setupControlButton("assets/icons/shuffle.png");
    connect(shuffleButton, &QPushButton::clicked, this, &ControlsView::shuffle);

    loopButton = setupControlButton("assets/icons/loop.png");
    connect(loopButton, &QPushButton::clicked, this, &ControlsView::loop);

    buttonsLayout->addStretch();
    
    volumeDownButton = setupControlButton("assets/icons/volume-decrease.png");
    connect(volumeDownButton, &QPushButton::clicked, this, &ControlsView::volumeDown);

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setFixedWidth(100);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    connect(volumeSlider, &QSlider::valueChanged, this, &ControlsView::setVolume);

    buttonsLayout->addWidget(volumeSlider);
    
    volumeUpButton = setupControlButton("assets/icons/volume-increase.png");
    connect(volumeUpButton, &QPushButton::clicked, this, &ControlsView::volumeUp);

    layout = new QVBoxLayout;
    layout->addLayout(playerLayout);
    layout->addLayout(buttonsLayout);
    root->setLayout(layout);
}

void ControlsView::refresh() {
    currentTimeLabel->setText(app->audioPlayer().hasFile() ? formatTime(round(app->audioPlayer().time())) : "--:--");
    remainingTimeLabel->setText(app->audioPlayer().hasFile() ? formatTime(round(app->audioPlayer().duration() - app->audioPlayer().time())) : "--:--");
    playButton->setEnabled(app->audioPlayer().hasFile() && !app->audioPlayer().isPlaying());
    pauseButton->setEnabled(app->audioPlayer().isPlaying());
    stopButton->setEnabled(app->audioPlayer().hasFile());
    nextButton->setEnabled(app->audioPlayer().hasNext());
    previousButton->setEnabled(app->audioPlayer().hasPrevious());
    playerSlider->setEnabled(app->audioPlayer().hasFile());
    volumeSlider->setValue(app->audioPlayer().volume() * 100);

    playerSlider->blockSignals(true);
    playerSlider->setValue(1000 * app->audioPlayer().progress());
    playerSlider->blockSignals(false);
}

QPushButton* ControlsView::setupControlButton(QString icon) {
    auto btn = new QPushButton;
    btn->setFixedSize(QSize(32, 32));
    btn->setIcon(QIcon(icon));
    btn->setIconSize(QSize(16, 16));
    btn->setStyleSheet(R"(
        QPushButton {
            border-style: outset; 
            border-width: 2px; 
            border-color: #CCCCCC
        }
        QPushButton:disabled {
            background-color: #BBB;
            border-color: #EEEEEE;
            opacity: 0.5;
        }
    )");
    buttonsLayout->addWidget(btn);
    return btn;
}

QString ControlsView::formatTime(double timeInSeconds) {
    std::string minutes = std::to_string((int)(timeInSeconds / 60));
    int seconds = (int)(timeInSeconds) % 60;
    std::string secondsString = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);
    return QString::fromStdString(minutes) + ":" + QString::fromStdString(secondsString);
}

void ControlsView::play() {
    app->audioPlayer().play();
    app->ui().refreshAll();
}

void ControlsView::pause() {
    app->audioPlayer().pause();
    app->ui().refreshAll();
}

void ControlsView::stop() {
    app->audioPlayer().stop();
    app->ui().refreshAll();
}

void ControlsView::next() {
    app->audioPlayer().next();
    app->ui().refreshAll();
}

void ControlsView::previous() {
    app->audioPlayer().previous();
    app->ui().refreshAll();
}

void ControlsView::shuffle() {
    app->audioPlayer().toggleShuffling();
    app->ui().refreshAll();
}

void ControlsView::loop() {
    app->audioPlayer().toggleLoop();
    app->ui().refreshAll();
}

void ControlsView::volumeUp() {
    float volume = app->audioPlayer().volume();
    app->audioPlayer().setVolume(volume + 0.1f <= 1.0f ? volume + 0.1f : 1.0f);
    app->ui().refreshAll();
}

void ControlsView::volumeDown() {
    float volume = app->audioPlayer().volume();
    app->audioPlayer().setVolume(volume - 0.1f >= 0.0f ? volume - 0.1f : 0.0f);
    app->ui().refreshAll();
}

void ControlsView::setVolume(int progress) {
    app->audioPlayer().setVolume((float)progress / 100.0f);
    app->ui().refreshAll();
}

void ControlsView::jumpAt(int progress) {
    app->audioPlayer().jumpAt(app->audioPlayer().duration() * (float)progress / 1000.0f);
    app->ui().refreshAll();
}