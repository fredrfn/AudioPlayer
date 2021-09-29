#define AUDIO_PLAYER_GUI_CLIENT 1

#if AUDIO_PLAYER_GUI_CLIENT
#include "app.hpp"
#include <QApplication>

#define TESTS_ENABLED 1

#if TESTS_ENABLED
    #define CATCH_CONFIG_RUNNER
    #include "catch.hpp"
#endif

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
#else 

#include "sounds/sound_player.hpp"
#include "sounds/sounds/files/mp3_file.hpp"
#include "sounds/sounds/synth/basic/sine_wave.hpp"
#include <iostream>

int main() {
    //MP3File file("C:/Users/fredr/Downloads/Rejuvenation - v13/Audio/BGM/Battle - Conclusive.mp3");
    //SoundPlayer player(file);
    Pitch frequency = PitchData("A4").frequency();
    SineWave sine(frequency);
    SoundPlayer player(sine);
    player.play();
    while(player.time() < 10.0) {}
    return 0;
}

#endif 
