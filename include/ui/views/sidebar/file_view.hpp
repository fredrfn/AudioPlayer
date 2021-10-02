#ifndef UI_VIEWS_SIDEBAR_FILE_VIEW_HPP
#define UI_VIEWS_SIDEBAR_FILE_VIEW_HPP

#include "ui/qt_view.hpp"
#include <string>
#include <QStandardItem>

class QTableView;

class FileView : public QtView {
    QTableView* tableView;
    QStandardItemModel model;
    std::string currentFile;
    double currentDuration;

    virtual void init() override;
    virtual void refresh() override;
};

#endif