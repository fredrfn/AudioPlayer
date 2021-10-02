#ifndef UI_VIEWS_PLAYER_VISUALIZERS_SIGNAL_WAVE_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_SIGNAL_WAVE_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QLabel>

class SignalWaveView : public QtView {
    virtual void init() override { 
        root = new QLabel("Signal wave view");
    }
};

#endif