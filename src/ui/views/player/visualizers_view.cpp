#include "ui/views/player/visualizers_view.hpp"
#include <QTabWidget>
#include "app.hpp"
#include <iostream>

void VisualizersView::init() {
    root = new QTabWidget(parent->widget(parent));
    ((QTabWidget*)root)->addTab(samples.widget(this), tr("Sound"));
    ((QTabWidget*)root)->addTab(amplitudes.widget(this), tr("Amplitudes"));
    ((QTabWidget*)root)->addTab(phases.widget(this), tr("Phases"));
}

void VisualizersView::refresh() {}