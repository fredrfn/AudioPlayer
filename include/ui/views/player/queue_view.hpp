#ifndef QUEUE_VIEW_HPP
#define QUEUE_VIEW_HPP

#include "ui/qt_view.hpp"

class QVBoxLayout;

class QueueView : public QtView {
    QVBoxLayout* layout;
protected: 
    virtual void init();
public:
    virtual void refresh();
};

#endif 