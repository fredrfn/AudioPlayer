#ifndef UI_VIEWS_PLAYER_VISUALIZERS_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_VIEW_HPP

#include "ui/qt_view.hpp"
#include "ui/views/player/visualizers/temporal_signal_view.hpp"
#include "ui/views/player/visualizers/spectrum_view.hpp"

class QTabWidget;

class VisualizersView : public QtView {
    QTabWidget* tabWidget;
    TemporalSignalView samples;
    SpectrumView amplitudes = {SpectrumViewType::AMPLITUDE};
    SpectrumView phases = {SpectrumViewType::PHASE};
    bool isAnalyzerInit = false;
    virtual void init() override;
    virtual void refresh() override;
public:
    virtual std::vector<QtView*> children() { return {&samples, &amplitudes, &phases}; }
};

#endif 