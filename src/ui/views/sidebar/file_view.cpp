#include "ui/views/sidebar/file_view.hpp"
#include "ui/helpers/strings_helper.hpp"
#include "app.hpp"

#include <vector>
#include <array>
#include <QGroupBox>
#include <QStandardItem>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>

void FileView::init() {
    root = new QGroupBox(tr("File properties"));
    QVBoxLayout* layout = new QVBoxLayout(root);

    tableView = new QTableView(root);
    tableView->verticalHeader()->hide();
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->hide();
    tableView->setSelectionMode(QTableView::NoSelection);
    tableView->setModel(&model);

    layout->addWidget(tableView);
    ((QGroupBox*)root)->setLayout(layout);

    root->setMaximumHeight(252);
}

void FileView::refresh() {
    if (currentFile == app->audioPlayer().current() && currentDuration == app->audioPlayer().duration()) {
        return;
    } 
    currentFile = app->audioPlayer().current();
    currentDuration = app->audioPlayer().duration();
    tableView->reset();
    model.clear();
    bool hasFile = app->audioPlayer().current().size() != 0;
    tableView->setDisabled(!hasFile);
    std::vector<std::array<std::string, 2>> rows = {
        { "Name", StringsHelper::extractFileName(currentFile) },
        { "Type", StringsHelper::extractFileExtension(currentFile) },
        { "Size", StringsHelper::formatFileSizeInMB(app->audioPlayer().size()) },
        { "Duration", StringsHelper::formatTime(currentDuration) },
        { "Sampling rate", std::to_string(app->audioPlayer().samplingRate()) + "Hz" },
        { "Channels", std::to_string(app->audioPlayer().channels()) },
        { "Path", currentFile }
    };
    for (const std::array<std::string, 2> &row : rows) {
        QList<QStandardItem*> items;
        auto header = new QStandardItem(tr(row[0].c_str()));
        QFont font = header->font();
        font.setBold(true);
        header->setFont(font);
        items.append(header);
        items.append(new QStandardItem(hasFile ? row[1].c_str() : ""));
        model.appendRow(items);
    }
    tableView->setModel(&model);
}