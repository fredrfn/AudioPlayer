#include "ui/views/player_view.hpp"

#include <QWidget>
#include <QHBoxLayout>

void PlayerView::init() {
    root = new QWidget;
    layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(controls.widget());
    root->setLayout(layout);
}

void PlayerView::refresh() {}