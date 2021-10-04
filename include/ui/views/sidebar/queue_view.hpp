#ifndef UI_VIEWS_SIDEBAR_QUEUE_VIEW_HPP
#define UI_VIEWS_SIDEBAR_QUEUE_VIEW_HPP

#include "ui/qt_view.hpp"

class QListWidget;
class QListWidgetItem;

class QueueView : public QtView {
    QListWidget* list;
    std::vector<QListWidgetItem*> items;
    virtual void init() override;
    virtual void refresh() override;
private slots:
    void fileClicked(QListWidgetItem* item);
};

#endif