#ifndef UI_VIEWS_PLAYER_VISUALIZERS_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_VIEW_HPP

#include "ui/qt_view.hpp"
#include "ui/views/player/visualizers/signal_wave_view.hpp"
#include "ui/views/player/visualizers/spectrum_view.hpp"

class QTabWidget;

class VisualizersView : public QtView {
    QTabWidget* tabWidget;
    SignalWaveView samples;
    SpectrumView amplitudes = {SpectrumViewType::AMPLITUDES};
    SpectrumView phases = {SpectrumViewType::PHASES};
    virtual void init() override;
public:
    virtual std::vector<QtView*> children() { return {&samples, &amplitudes, &phases}; }
};

#endif 