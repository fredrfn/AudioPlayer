#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "ui/qt_view.hpp"
#include "ui/views/menu_view.hpp"
#include "ui/views/player_view.hpp"
#include <QThread>
#include <thread>

class QWidget;
class QVBoxLayout;
class App;

class MainWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT
    QWidget* mainWidget;
    QVBoxLayout* mainLayout;
    PlayerView playerView;
    MenuView menuView; 
    App* app;  
    std::thread periodicRefresher;
public slots:
    void refreshAll();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setApp(App* application);
    static void periodicRefresh(MainWindow* context);
signals:    
    void refresh();
};

#endif
