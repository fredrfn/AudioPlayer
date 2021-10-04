#ifndef UI_VIEWS_PLAYER_EFFECTS_ECHO_VIEW_HPP
#define UI_VIEWS_PLAYER_EFFECTS_ECHO_VIEW_HPP

#include "ui/qt_view.hpp"

class QSpinBox;
class QSlider;
class QCheckBox;

class EchoView : public QtView {
    QSpinBox* echoes;
    QSlider* spacing;
    QSlider* gain;
    QCheckBox* toggler;
    virtual void init() override;
public slots: 
    void setSpacing(int value);
    void setGain(int value);
    void setEchoes(int value);
    void toggle();
};

#endif