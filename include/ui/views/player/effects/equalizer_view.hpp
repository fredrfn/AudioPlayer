#ifndef UI_VIEWS_PLAYER_EFFECTS_EQUALIZER_VIEW_HPP
#define UI_VIEWS_PLAYER_EFFECTS_EQUALIZER_VIEW_HPP

#include "ui/qt_view.hpp"
#include <vector>

class QSlider;
class QHBoxLayout;
class QCheckBox;
class EqualizerView : public QtView {
    std::vector<QSlider*> sliders;
    QCheckBox* toggler;
    QCheckBox* groupControl;
    QHBoxLayout* slidersLayout;
    QWidget* slidersLayoutContainer;
    virtual void init() override;
    virtual void refresh() override;
 public slots:
    void updateEqualizer(int value);
    void toggle();
};

#endif