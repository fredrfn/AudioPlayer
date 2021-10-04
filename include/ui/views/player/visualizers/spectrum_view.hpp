#ifndef UI_VIEWS_PLAYER_VISUALIZERS_SPECTRUM_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_SPECTRUM_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QVector>
#include <QPointF>
#include "sounds/maths/complex.hpp"
#include <vector>

class QChartView;
class QChart;
class QLineSeries;
class QDateTimeAxis;
class SoundBuffer;

enum SpectrumViewType { AMPLITUDE, PHASE };
class SpectrumView : public QtView {
    SpectrumViewType type;
    QChart* chart;
    QLineSeries* series;
    QVector<QPointF> accumulator;
    bool hasChanges = false;
    std::vector<Complex> input;
    virtual void init() override;
    virtual void refresh() override;
public:
    SpectrumView(SpectrumViewType type) : type(type) {}
    void gatherSamples(const SoundBuffer& buffer);
};

#endif