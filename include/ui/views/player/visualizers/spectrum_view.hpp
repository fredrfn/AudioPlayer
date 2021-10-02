#ifndef UI_VIEWS_PLAYER_VISUALIZERS_SPECTRUM_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_SPECTRUM_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QLabel>

enum SpectrumViewType { AMPLITUDES, PHASES };

class SpectrumView : public QtView {
    SpectrumViewType type;
    virtual void init() override { 
        root = new QLabel(type == AMPLITUDES ? "Amplitudes" : "Phases");
    }
public:
    SpectrumView(SpectrumViewType type) : QtView(), type(type) {}
};

#endif