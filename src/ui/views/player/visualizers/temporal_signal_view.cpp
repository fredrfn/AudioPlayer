#include "ui/views/player/visualizers/temporal_signal_view.hpp"

#include <QChartView>
#include <QLineSeries>
#include "app.hpp"

void TemporalSignalView::init() {
    chart = new QChart();
    chart->legend()->hide();
    series = new QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes()[1]->setRange(-1.0f, 1.0f);
    chart->axes()[0]->hide();
    chart->axes()[1]->hide();
    chart->setContentsMargins(0,0,0,0);
    root = new QChartView(chart);
    root->setContentsMargins(0, 0, 0, 0);
    ((QChartView*)root)->setRenderHint(QPainter::Antialiasing);
}

void TemporalSignalView::refresh() {
    if (isFirstRefresh) {
        accumulator.reserve(24000);
    }
    chart->blockSignals(true);
    series->replace(accumulator);
    chart->axes()[0]->setRange(0, accumulator.count());
    accumulator.clear();
    chart->blockSignals(false);
}

void TemporalSignalView::gatherSamples(const SoundBuffer& buffer) {
    for(SampleCount sample = 0; sample < buffer.channelLength(); sample+=6) {
        accumulator.push_back(QPointF(accumulator.count(), buffer.read(sample, 0)));
    }
}
