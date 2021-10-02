#ifndef UI_VIEWS_PLAYER_VISUALIZERS_TEMPORAL_SIGNAL_VIEW_HPP
#define UI_VIEWS_PLAYER_VISUALIZERS_TEMPORAL_SIGNAL_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QVector>
#include <QPointF>

class QChartView;
class QChart;
class QLineSeries;
class QDateTimeAxis;
class SoundBuffer;

class TemporalSignalView : public QtView {
    QChart* chart;
    QLineSeries* series;
    bool hasChanges = false;
    bool isInitialized = false;
    QVector<QPointF> accumulator;
    virtual void init() override;
    virtual void refresh() override;
public:
    void gatherSamples(const SoundBuffer& buffer);
};

#endif