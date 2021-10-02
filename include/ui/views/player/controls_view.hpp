#ifndef UI_VIEWS_PLAYER_CONTROLS_VIEW_HPP
#define UI_VIEWS_PLAYER_CONTROLS_VIEW_HPP

#include "ui/qt_view.hpp"

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QSlider;
class QLabel;

class ControlsView : public QtView {
    QVBoxLayout* layout;

    QHBoxLayout* playerLayout;
    QLabel* currentTimeLabel;
    QSlider* playerSlider;
    QLabel* remainingTimeLabel;

    QHBoxLayout* buttonsLayout;
    QPushButton* playButton;
    QPushButton* stopButton;
    QPushButton* previousButton;
    QPushButton* nextButton;
    QPushButton* shuffleButton;
    QPushButton* loopButton;
    QPushButton* volumeUpButton;
    QPushButton* volumeDownButton;
    QSlider* volumeSlider;

    QPushButton* setupControlButton(QString icon);
    QString buttonStylesheet(bool active = false);
    QString sliderStylesheet(QString color1, QString color2);
protected: 
    virtual void init();
    virtual void refresh();
private slots:
    void togglePlay();
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