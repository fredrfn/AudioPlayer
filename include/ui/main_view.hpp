#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "ui/qt_view.hpp"
#include "ui/views/menu_view.hpp"
#include "ui/views/player_view.hpp"
#include "ui/views/sidebar_view.hpp"
#include <thread>

class QWidget;
class QVBoxLayout;
class App;
class QSplitter;

class MainWindow : public QMainWindow {
    Q_OBJECT
    QtView* view;
public slots:
    void refreshAll() { view->refreshAll(); }
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setView(QtView* v) { view = v; };
signals:    
    void refresh();
};

class MainView : public QtView {
    QVBoxLayout* mainLayout;
    MenuView menu; 
    QSplitter* splitter;
    SidebarView sidebar;
    PlayerView player;
    virtual void init() override;
    virtual void refresh() override;
public:
    MainView() { mainWindow.setView(this); widget(nullptr); }
    MainWindow mainWindow;
    virtual std::vector<QtView*> children() override { return { &menu, &sidebar, &player }; }
};

#endif
