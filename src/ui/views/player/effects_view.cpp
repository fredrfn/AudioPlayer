#include "ui/views/player/effects_view.hpp"
#include <QHBoxLayout>
#include <QFrame>

void EffectsView::init() {
    root = new QFrame;
    layout = new QHBoxLayout(root);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(equalizer.widget(this));
    layout->addWidget(echo.widget(this));
    root->setLayout(layout);
}