#ifndef QT_VIEW_HPP
#define QT_VIEW_HPP

#include <QObject>
#include <vector>

class QWidget;
class App;

class QtView : public QObject {
    Q_OBJECT
protected:
    App* app;
    QWidget* root = nullptr;
    virtual void init() = 0; // responsible for setup/allocating widgets/children
public:
    QWidget* widget() {
        if (root == nullptr) { init(); }
        return root;
    }
    virtual std::vector<QtView*> children() { return {}; }
    void setApp(App* application) { 
        app = application; 
        for(auto& child : children()) { child->setApp(application); } 
    }
    virtual void refresh() = 0;
    void refreshAll() {
        refresh();
        for(auto& child : children()) { child->refreshAll(); } 
    }
};

#endif