#ifndef UI_VIEWS_SIDEBAR_VIEW_HPP
#define UI_VIEWS_SIDEBAR_VIEW_HPP

#include "ui/qt_view.hpp"
#include "ui/views/sidebar/file_view.hpp"
#include "ui/views/sidebar/queue_view.hpp"
#include "ui/stylesheets.hpp"
#include <QSplitter>

class SidebarView : public QtView {
    FileView file;
    QueueView queue;
    virtual void init() override { 
        root = new QSplitter(Qt::Vertical, parent->widget(parent)); 
        root->setStyleSheet(QString::fromStdString(Stylesheets::splitter()));
        ((QSplitter*)root)->addWidget(file.widget(parent));
        ((QSplitter*)root)->addWidget(queue.widget(parent));
    }
public:
    virtual std::vector<QtView*> children() override { return { &file, &queue }; }
};

#endif