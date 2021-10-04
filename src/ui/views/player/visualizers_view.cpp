#include "ui/views/player/visualizers_view.hpp"
#include <QTabWidget>
#include "app.hpp"
#include <iostream>

void VisualizersView::init() {
    root = new QTabWidget(parent->widget(parent));
    ((QTabWidget*)root)->addTab(samples.widget(this), tr("Sound"));
    //((QTabWidget*)root)->addTab(amplitudes.widget(this), tr("Amplitudes"));
    //((QTabWidget*)root)->addTab(phases.widget(this), tr("Phases"));
    root->setMinimumHeight(250);
}

void VisualizersView::refresh() {
    app->audioCallbackContext = this;
    app->audioCallback = [](void* ctx, const SoundBuffer& buffer) {
        auto visualizers = reinterpret_cast<VisualizersView*>(ctx);
        switch (((QTabWidget*)visualizers->root)->currentIndex()) {
            case 0: visualizers->samples.gatherSamples(buffer); break;
            case 1: visualizers->amplitudes.gatherSamples(buffer); break;
            case 2: visualizers->phases.gatherSamples(buffer); break;
            default: break;
        } 
    };
}