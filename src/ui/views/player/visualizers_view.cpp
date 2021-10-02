#include "ui/views/player/visualizers_view.hpp"
#include <QTabWidget>

void VisualizersView::init() {
    root = new QTabWidget(parent->widget(parent));
    ((QTabWidget*)root)->addTab(samples.widget(this), tr("Sound samples"));
    ((QTabWidget*)root)->addTab(amplitudes.widget(this), tr("Amplitudes spectrum"));
    ((QTabWidget*)root)->addTab(phases.widget(this), tr("Phases spectrum"));
}