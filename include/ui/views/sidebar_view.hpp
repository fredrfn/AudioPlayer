#ifndef UI_VIEWS_SIDEBAR_VIEW_HPP
#define UI_VIEWS_SIDEBAR_VIEW_HPP

#include "ui/qt_view.hpp"
#include "ui/views/sidebar/file_view.hpp"
#include "ui/views/sidebar/queue_view.hpp"
#include <QSplitter>

class SidebarView : public QtView {
    FileView file;
    QueueView queue;
    virtual void init() override { 
        auto splitter = new QSplitter(parent->widget(parent)); 
        root = splitter;
        splitter->setOrientation(Qt::Vertical);
        splitter->addWidget(file.widget(parent));
        splitter->addWidget(queue.widget(parent));
    }
public:
    virtual std::vector<QtView*> children() override { return { &file, &queue }; }
};

#endif