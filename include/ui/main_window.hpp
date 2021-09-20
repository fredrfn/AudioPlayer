#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "ui/qt_view.hpp"
#include "ui/views/menu_view.hpp"
#include "ui/views/player_view.hpp"
#include <thread>

class QWidget;
class QVBoxLayout;
class App;

class MainWindow : public QMainWindow {
    Q_OBJECT
    QWidget* mainWidget;
    QVBoxLayout* mainLayout;
    PlayerView playerView;
    MenuView menuView; 
    App* app;  
    std::thread periodicRefresher;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setApp(App* application);
    void refreshAll();
    static void periodicRefresh(MainWindow* context);
};

#endif
