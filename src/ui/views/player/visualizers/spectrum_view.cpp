#include "ui/views/player/visualizers/temporal_signal_view.hpp"
#include "sounds/maths/fourier.hpp"
#include "sounds/maths/commons.hpp"
#include <QChartView>
#include <QLineSeries>
#include "app.hpp"

void SpectrumView::init() {
    chart = new QChart();
    chart->legend()->hide();
    series = new QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes()[0]->hide();
    chart->axes()[1]->hide();
    input.reserve(44000);
    if (type == SpectrumViewType::AMPLITUDE) {
        chart->axes()[1]->setRange(-PI/2, PI/2);
    }
    chart->setContentsMargins(0,0,0,0);
    root = new QChartView(chart);
    root->setContentsMargins(0, 0, 0, 0);
    ((QChartView*)root)->setRenderHint(QPainter::Antialiasing);
}

void SpectrumView::refresh() {
    if (isFirstRefresh) {
        accumulator.reserve(24000);
    }
    if (hasChanges) {
        chart->blockSignals(true);
        series->replace(accumulator);
        chart->axes()[0]->setRange(0, accumulator.count());
        accumulator.clear();
        chart->blockSignals(false);
        hasChanges = false;
    } 
}

void SpectrumView::gatherSamples(const SoundBuffer& buffer) {
    for(SampleCount sample = 0; sample < buffer.channelLength(); sample++) {
        input.push_back(Complex(buffer.read(sample, 0)));
    }
    std::vector<Complex> output = fft(input);
    if (type == SpectrumViewType::AMPLITUDE) {
        for(SampleCount sample = 0; sample < output.size(); sample+=10) {
            accumulator.push_back(QPointF(accumulator.count(), output[sample].modulus()));
        }
    } else {
        for(SampleCount sample = 0; sample < output.size(); sample+=10) {
            accumulator.push_back(QPointF(accumulator.count(), output[sample].argument()));
        }
    }
    hasChanges = true;
  
}
