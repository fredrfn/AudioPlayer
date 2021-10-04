#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "ui/qt_view.hpp"
#include "ui/views/menu_view.hpp"
#include "ui/views/player_view.hpp"
#include "ui/views/sidebar_view.hpp"
#include "sounds/sound_buffer.hpp"
#include <thread>

class QWidget;
class QVBoxLayout;
class App;
class QSplitter;
class MainView;

class MainWindow : public QMainWindow {
    Q_OBJECT
    MainView* view;
public slots:
    void refreshAll(SoundBuffer);
    void fireFilesChangedCallback();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setView(MainView* v) { view = v; };
signals:    
    void refresh(SoundBuffer);
    void fireFilesChanged();
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
    void processAudioEvent(SoundBuffer buffer);
    void fireFilesChanged();
};

#endif
