#ifndef UI_VIEWS_SIDEBAR_FILE_VIEW_HPP
#define UI_VIEWS_SIDEBAR_FILE_VIEW_HPP

#include "ui/qt_view.hpp"
#include <QLabel>

class FileView : public QtView {
    virtual void init() override { 
        root = new QLabel("File property view");
        ((QLabel*)root)->setFrameStyle(QFrame::Panel | QFrame::Raised);
        ((QLabel*)root)->setLineWidth(2);
    }
};

#endif