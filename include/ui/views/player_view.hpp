#ifndef UI_VIEWS_PLAYER_VIEW_HPP
#define UI_VIEWS_PLAYER_VIEW_HPP

#include "ui/qt_view.hpp"
#include "ui/views/player/controls_view.hpp"
#include "ui/views/player/effects_view.hpp"
#include "ui/views/player/visualizers_view.hpp"

class QSplitter;
class QVBoxLayout;
class QWidget;

class PlayerView : public QtView {
    QSplitter* splitter;
    VisualizersView visualizers;
    QWidget* controlEffectsContainer;
    QVBoxLayout* controlEffectsLayout;
    ControlsView controls;
    EffectsView effects;
    virtual void init() override;
public:
    virtual std::vector<QtView*> children() { return {&visualizers, &controls, &effects}; }
};

#endif 