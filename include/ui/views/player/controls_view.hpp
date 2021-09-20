#ifndef CONTROLS_VIEW_HPP
#define CONTROLS_VIEW_HPP

#include "ui/qt_view.hpp"

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QSlider;
class QLabel;
class AudioPlayer;

class ControlsView : public QtView {
    QVBoxLayout* layout;

    QHBoxLayout* playerLayout;
    QLabel* currentTimeLabel;
    QSlider* playerSlider;
    QLabel* remainingTimeLabel;

    QHBoxLayout* buttonsLayout;
    QPushButton* playButton;
    QPushButton* pauseButton;
    QPushButton* stopButton;
    QPushButton* previousButton;
    QPushButton* nextButton;
    QPushButton* shuffleButton;
    QPushButton* loopButton;
    QPushButton* volumeUpButton;
    QPushButton* volumeDownButton;
    QSlider* volumeSlider;

    QPushButton* setupControlButton(QString icon);
protected: 
    virtual void init();
public:
    virtual void refresh();
private slots:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void shuffle();
    void loop();
    void volumeUp();
    void volumeDown();
    QString formatTime(double timeInSeconds);
    void jumpAt(int progress);
    void setVolume(int progress);
};

#endif 