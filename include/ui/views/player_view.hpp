#ifndef PLAYER_VIEW_HPP
#define PLAYER_VIEW_HPP


#include "ui/qt_view.hpp"
#include "ui/views/player/controls_view.hpp"
#include "ui/views/player/queue_view.hpp"

class QHBoxLayout;
class AudioPlayer;

class PlayerView : public QtView {
    ControlsView controls;
    QHBoxLayout* layout;
protected: 
    virtual void init();
public:
    virtual void refresh();
    virtual std::vector<QtView*> children() { return {&controls}; };
};

#endif 