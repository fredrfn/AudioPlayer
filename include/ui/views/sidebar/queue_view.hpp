#ifndef UI_VIEWS_SIDEBAR_QUEUE_VIEW_HPP
#define UI_VIEWS_SIDEBAR_QUEUE_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QLabel>

class QueueView : public QtView {
    virtual void init() override { 
        root = new QLabel("Queue view");
        ((QLabel*)root)->setFrameStyle(QFrame::Panel | QFrame::Raised);
        ((QLabel*)root)->setLineWidth(2);
    }
};

#endif