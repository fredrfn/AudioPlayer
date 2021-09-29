#define TESTS_ENABLED 1

#if TESTS_ENABLED
    #define CATCH_CONFIG_RUNNER
    #include "catch.hpp"
#endif

#include "app.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {
    #if TESTS_ENABLED
        int result = Catch::Session().run( argc, argv );
        if (result != 0) {
            return result;
        }
    #endif

    QApplication qtApplication(argc, argv);
    return App(qtApplication).run();
}
