#ifndef UI_VIEWS_PLAYER_VISUALIZERS_SPECTRUM_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_SPECTRUM_VIEW_HPP

#include "ui/qt_view.hpp"
#include "sounds/maths/complex.hpp"

enum SpectrumViewType { AMPLITUDES, PHASES };

class QChartView;
class QChart;
class QLineSeries;

class SpectrumView : public QtView {
    SpectrumViewType type;
    QChart* chart;
    QLineSeries* series;
    std::string currentFile;
    virtual void init() override;
    virtual void refresh() override;
public:
    SpectrumView(SpectrumViewType type) : QtView(), type(type) {}
    void update(std::vector<Complex>& values);
};

#endif