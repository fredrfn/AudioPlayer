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
#include <string>

void ControlsView::init() {
    root = new QWidget;
    
    playerLayout = new QHBoxLayout;

    currentTimeLabel = new QLabel("--:--");
    playerLayout->addWidget(currentTimeLabel);

    playerSlider = new QSlider(Qt::Horizontal);
    playerSlider->setStyleSheet(sliderStylesheet("#4444f4", "#8888f8"));
    playerSlider->setMinimum(0);
    playerSlider->setMaximum(1000);
    connect(playerSlider, &QSlider::valueChanged, this, &ControlsView::jumpAt);

    playerLayout->addWidget(playerSlider);

    remainingTimeLabel = new QLabel("--:--");
    playerLayout->addWidget(remainingTimeLabel);

    buttonsLayout = new QHBoxLayout;

    playButton = setupControlButton("assets/icons/play3.png");
    connect(playButton, &QPushButton::clicked, this, &ControlsView::togglePlay);

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
    volumeSlider->setStyleSheet(sliderStylesheet("#44f444", "#88f888"));

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
    currentTimeLabel->setText(!app->audioPlayer().isEmpty() ? formatTime(round(app->audioPlayer().time())) : "--:--");
    remainingTimeLabel->setText(!app->audioPlayer().isEmpty() ? formatTime(round(app->audioPlayer().duration() - app->audioPlayer().time())) : "--:--");
    playButton->setEnabled(!app->audioPlayer().isEmpty());
    if (app->audioPlayer().isPlaying()) {
        playButton->setIcon(QPixmap("assets/icons/pause2.png"));
    } else {
        playButton->setIcon(QPixmap("assets/icons/play3.png"));
    }
    playButton->setStyleSheet(buttonStylesheet(app->audioPlayer().isPlaying()));
    stopButton->setEnabled(!app->audioPlayer().isEmpty());
    nextButton->setEnabled(app->audioPlayer().hasNext());
    previousButton->setEnabled(app->audioPlayer().hasPrevious());
    loopButton->setStyleSheet(buttonStylesheet(app->audioPlayer().isLooping()));
    shuffleButton->setStyleSheet(buttonStylesheet(app->audioPlayer().isShuffling()));
    playerSlider->setEnabled(!app->audioPlayer().isEmpty());
    volumeSlider->setValue(app->processors().amplifier().gain() * 100);
    playerSlider->blockSignals(true);
    playerSlider->setValue(1000 * app->audioPlayer().progress());
    playerSlider->blockSignals(false);
}

QString ControlsView::buttonStylesheet(bool active) {
    QString borderColor = active ? "#4444d4" : "#CCCCCC";
    return R"(
        QPushButton {
            border-style: outset; 
            border-width: 2px; 
            border-color:)" + borderColor + R"(;
        }
        QPushButton:disabled {
            background-color: #BBB;
            border-color: #EEEEEE;
            opacity: 0.5;
        }
    )";
}

QString ControlsView::sliderStylesheet(QString color1, QString color2) {
    return R"(
        QSlider::groove:horizontal {
            border: 1px solid #cccccc;
            height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d4d4d4, stop:1 #f8f8f8);
        }
        QSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 )" + color1 + ", stop:1 " + color2 + R"();
            height: 6px;
            margin-top: 1px;
        }
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);
            border: 1px solid #cccccc;
            width: 12px;
            height: 10px;
            margin-left: -1px;
            margin-bottom: -1px;
        }
    )";
}

QPushButton* ControlsView::setupControlButton(QString icon) {
    auto btn = new QPushButton;
    btn->setFixedSize(QSize(32, 32));
    // btn->setIcon(QIcon(icon)); This creates a memory leak!!
    btn->setIcon(QPixmap(icon));
    btn->setIconSize(QSize(16, 16));
    btn->setStyleSheet(buttonStylesheet(false));
    buttonsLayout->addWidget(btn);
    return btn;
}

QString ControlsView::formatTime(double timeInSeconds) {
    std::string minutes = std::to_string((int)(timeInSeconds / 60));
    int seconds = (int)(timeInSeconds) % 60;
    std::string secondsString = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);
    return QString::fromStdString(minutes) + ":" + QString::fromStdString(secondsString);
}

void ControlsView::togglePlay() {
    if (app->audioPlayer().isPlaying()) {
        app->audioPlayer().pause();
    } else {
        app->audioPlayer().play();
    }
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
    float volume = app->processors().amplifier().gain();
    app->processors().amplifier().gain(volume + 0.1f <= 1.0f ? volume + 0.1f : 1.0f);
    app->ui().refreshAll();
}

void ControlsView::volumeDown() {
    float volume = app->processors().amplifier().gain();
    app->processors().amplifier().gain(volume - 0.1f >= 0.0f ? volume - 0.1f : 0.0f);
    app->ui().refreshAll();
}

void ControlsView::setVolume(int progress) {
    app->processors().amplifier().gain((float)progress / 100.0f);
    app->ui().refreshAll();
}

void ControlsView::jumpAt(int progress) {
    app->audioPlayer().jumpAt(app->audioPlayer().duration() * (float)progress / 1000.0f);
    app->ui().refreshAll();
}