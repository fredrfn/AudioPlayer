#include "ui/views/player/visualizers/spectrum_view.hpp"

#include <QChartView>
#include <QLineSeries>
#include <QRandomGenerator>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>
#include "app.hpp"

void SpectrumView::init() {
    chart = new QChart();
    chart->legend()->hide();

    series = new QLineSeries();   
    chart->addSeries(series);
    chart->createDefaultAxes();

    root = new QChartView(chart);
    ((QChartView*)root)->setRenderHint(QPainter::Antialiasing);
}

void SpectrumView::refresh() {
    if (currentFile != app->audioPlayer().current()) {
        currentFile = app->audioPlayer().current();
        series->clear();
    } 
}

void SpectrumView::update([[maybe_unused]]std::vector<Complex>& fftOutput) {
    /*series->clear();
    for (unsigned long long f = 0; f < fftOutput.size(); f++) {
        series->append(f, (type==SpectrumViewType::AMPLITUDES) ? fftOutput[f].modulus() : fftOutput[f].argument());
    }*/
}