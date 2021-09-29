#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include <QObject>
#include "ui/qt_view.hpp"

class QMenuBar;
class QMenu;
class QAction;

class MenuView : public QtView {
    QMenuBar* menubar;
    QMenu* fileMenu;
    QMenu* playbackMenu;
    QMenu* audioMenu;
    QMenu* helpMenu;
    QAction* aboutAction;
    QAction* openAction;
    QAction* openMultipleAction;
    QAction* openFolderAction;
    QAction* exitAction;
    QAction* playAction;
    QAction* pauseAction;
    QAction* stopAction;
    QAction* previousAction;
    QAction* nextAction;
    QAction* loopAction;
    QAction* shuffleAction;
    QAction* volumeUpAction;
    QAction* volumeDownAction;
    QAction* muteAction;
    QString supportedFilesFormatString();
protected: 
    virtual void init();
public:
    virtual void refresh();
private slots:
    void about();
    void open();
    void openMultiple();
    void openFolder();
    void exit();
    void play();
    void pause();
    void stop();
    void previous();
    void next();
    void loop();
    void shuffle();
    void volumeUp();
    void volumeDown();
    void mute();
};

#endif 