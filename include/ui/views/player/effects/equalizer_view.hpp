#ifndef UI_VIEWS_PLAYER_EFFECTS_EQUALIZER_VIEW_HPP
#define UI_VIEWS_PLAYER_EFFECTS_EQUALIZER_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QLabel>

class EqualizerView : public QtView {
    virtual void init() override { 
        root = new QLabel("Equalizer");
        ((QLabel*)root)->setFrameStyle(QFrame::Panel | QFrame::Raised);
        ((QLabel*)root)->setLineWidth(2);
    }
};

#endif