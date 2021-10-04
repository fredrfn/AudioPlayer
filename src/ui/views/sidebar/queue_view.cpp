#include "ui/views/sidebar/queue_view.hpp"
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QGroupBox>
#include "app.hpp"
#include "ui/helpers/strings_helper.hpp"
#include <iostream>

void QueueView::init() { 
    root = new QGroupBox(tr("Enqueued files"));
    list = new QListWidget(root);
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(list);
    ((QGroupBox*)root)->setLayout(layout);
    connect(list, &QListWidget::itemClicked, this, &QueueView::fileClicked);
}

void QueueView::refresh() {
    if (isFirstRefresh) {
        app->filesChangedCallbackContext = this;
        app->filesChangedCallback = [](void* ctx) {
            auto queue = reinterpret_cast<QueueView*>(ctx);
            queue->list->clear();
            if (queue->items.size() > 0) {
                queue->items.clear();
            }
            unsigned int i = 0;
            for (auto& str : queue->app->audioPlayer().queuedFiles()) {
                auto item = new QListWidgetItem(QString::fromStdString(StringsHelper::extractFileName(str)));
                if (i == queue->app->audioPlayer().currentQueueIndex()) { 
                    item->setBackground(QBrush(QColor(0, 125, 220)));
                    item->setForeground(QBrush(QColor(255,255,255)));
                }
                queue->list->addItem(item);
                queue->items.push_back(item);
                i++;
            }
            queue->list->repaint();
        };
    }
}

void QueueView::fileClicked(QListWidgetItem* item) {
    auto itr = std::find(items.begin(), items.end(), item);
    if (itr == items.end()) {
        return;
    }
    unsigned int index = itr - items.begin();
    if (app->audioPlayer().currentQueueIndex() != (unsigned int)index) {
        app->audioPlayer().switchFile(index);
        app->fireFilesChangedCallback();
    }
}
