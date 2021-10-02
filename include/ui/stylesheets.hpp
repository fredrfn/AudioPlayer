#ifndef UI_VIEWS_STYLESHEETS_HPP
#define UI_VIEWS_STYLESHEETS_HPP

#include <string>

class Stylesheets {
public:
    static std::string splitter() {
        return R"(QSplitter::handle {
            background: #F8F8F8;
            border: 1px solid #CCCCCC;
            width: 2px;
        })";
    }
};

#endif