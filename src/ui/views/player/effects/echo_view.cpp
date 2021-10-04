#include "ui/views/player/effects/echo_view.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include "app.hpp"

void EchoView::init() {
    root = new QGroupBox("Echo");
    auto layout = new QVBoxLayout(root);
    // Toggle
    auto hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    auto hlayoutContainer = new QWidget(root);
    toggler = new QCheckBox;
    connect(toggler, &QCheckBox::stateChanged, this, &EchoView::toggle);
    hlayout->addWidget(toggler);
    hlayoutContainer->setLayout(hlayout);
    layout->addWidget(hlayoutContainer);
    hlayout->addWidget(new QLabel(tr("Toggle")), 1);
    // Echoes
    hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    hlayoutContainer = new QWidget(root);
    hlayout->addWidget(new QLabel(tr("Echoes")));
    echoes = new QSpinBox;
    echoes->setMinimum(0);
    echoes->setValue(3);
    echoes->setMaximum(6);
    echoes->setEnabled(false);
    connect(echoes, &QSpinBox::valueChanged, this, &EchoView::setEchoes);
    hlayout->addWidget(echoes);
    hlayoutContainer->setLayout(hlayout);
    layout->addWidget(hlayoutContainer);
    // Sliders
    hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    hlayoutContainer = new QWidget(root);
    // Strength slider
    auto vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0,0,0,0);
    auto vlayoutContainer = new QWidget(root);
    vlayout->addWidget(new QLabel(tr("Strength")));
    gain = new QSlider(Qt::Vertical);
    gain->setMinimum(0);
    gain->setValue(50);
    gain->setMaximum(100);
    gain->setEnabled(false);
    connect(gain, &QSlider::valueChanged, this, &EchoView::setGain);
    vlayout->addWidget(gain);
    vlayoutContainer->setLayout(vlayout);
    hlayout->addWidget(vlayoutContainer);
    // Delay slider
    vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0,0,0,0);
    vlayoutContainer = new QWidget(root);
    vlayout->addWidget(new QLabel(tr("Delay")));
    spacing = new QSlider(Qt::Vertical);
    spacing->setMinimum(0);
    spacing->setValue(50);
    spacing->setMaximum(100);
    spacing->setEnabled(false);
    connect(spacing, &QSlider::valueChanged, this, &EchoView::setSpacing);
    vlayout->addWidget(spacing);
    vlayoutContainer->setLayout(vlayout);
    hlayout->addWidget(vlayoutContainer);

    hlayoutContainer->setLayout(hlayout);
    layout->addWidget(hlayoutContainer);

    root->setLayout(layout);
}

void EchoView::setSpacing(int value) {
    app->processors().echo().spacing((double)value/50.0);
}

void EchoView::setGain(int value) {
    app->processors().echo().gain((float)value/100.0);
}

void EchoView::setEchoes(int value) {
    app->processors().echo().echoes((unsigned int)value);
}

void EchoView::toggle() {
    app->processors().echo().toggle();
    bool isEnabled = app->processors().echo().isEnabled();
    echoes->setEnabled(isEnabled);
    gain->setEnabled(isEnabled);
    spacing->setEnabled(isEnabled);
}