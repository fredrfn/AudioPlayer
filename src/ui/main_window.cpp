#include "ui/main_window.hpp"

#include <QLayout>
#include <QLayoutItem>
#include <QMenuBar>
#include "app.hpp"

#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(tr("AudioPlayer"));
    resize(500, 100);
    setMinimumSize(500, 100);
    setContentsMargins(0, 0, 0, 0);

    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(menuView.widget());
    mainLayout->addStretch();
    mainLayout->addWidget(playerView.widget());
    
    mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    connect(this, SIGNAL(refresh()), this, SLOT(refreshAll()));
}

MainWindow::~MainWindow() {}

void MainWindow::refreshAll() {
    setWindowTitle(tr("AudioPlayer") + QString::fromStdString(!app->audioPlayer.isEmpty() ? " - " + app->audioPlayer.current() : ""));
    menuView.refreshAll();
    playerView.refreshAll();
}

void MainWindow::setApp(App* application) {
    app = application;
    for (QtView* view : std::vector<QtView*>({&menuView, &playerView})) {
        view->setApp(application);
    }
}