#ifndef UI_VIEWS_PLAYER_EFFECTS_VIEW_HPP
#define UI_VIEWS_PLAYER_EFFECTS_VIEW_HPP

#include "ui/qt_view.hpp"
#include "ui/views/player/effects/echo_view.hpp"
#include "ui/views/player/effects/equalizer_view.hpp"

class QHBoxLayout;

class EffectsView : public QtView {
    QHBoxLayout* layout;
    EchoView echo;
    EqualizerView equalizer;
    virtual void init() override;
public:
    virtual std::vector<QtView*> children() { return {&echo, &equalizer}; }
};

#endif 