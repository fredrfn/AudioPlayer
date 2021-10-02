#ifndef QT_VIEW_HPP
#define QT_VIEW_HPP

#include <QObject>
#include <vector>

class App;
class QWidget;

class QtView : public QObject {
    Q_OBJECT
protected:
    App* app = nullptr;
    QWidget* root = nullptr;
    QtView* parent = nullptr;
    virtual void init() = 0; // responsible for setup/allocating widgets/children
    virtual void refresh() {}; // responsible for propagating app state to widgets
public:
    virtual std::vector<QtView*> children() { return {}; } // must contain the list of all subviews
    QWidget* widget(QtView* parentView) { // QtView -> QWidget transformation
        if (root == nullptr) { parent = parentView; init(); }
        return root;
    }
    void setApp(App* application) { // app context propagation
        app = application; 
        for(auto& child : children()) { child->setApp(application); } 
    }
    void refreshAll() { // this method synchronizes the app state to the ui widgets, recursively
        refresh();
        for(auto& child : children()) { child->refreshAll(); } 
    }
};

#endif