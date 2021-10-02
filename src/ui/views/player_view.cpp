#include "ui/views/player_view.hpp"
#include "ui/stylesheets.hpp"

#include <QSplitter>
#include <QVBoxLayout>

void PlayerView::init(){
    root = new QSplitter(Qt::Vertical, parent->widget(parent));
    root->setStyleSheet(QString::fromStdString(Stylesheets::splitter()));

    controlEffectsLayout = new QVBoxLayout(root);
    controlEffectsLayout->setContentsMargins(0, 0, 0, 0);

    controlEffectsContainer = new QWidget(root);

    controlEffectsLayout->addWidget(controls.widget(this));
    controlEffectsLayout->addWidget(effects.widget(this), 1);
    controlEffectsContainer->setLayout(controlEffectsLayout);

    ((QSplitter*)root)->addWidget(visualizers.widget(this));
    ((QSplitter*)root)->addWidget(controlEffectsContainer);
}
