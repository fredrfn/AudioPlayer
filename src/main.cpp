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
#include "sounds/sounds/synth/basic/fourier_wave.hpp"
#include "sounds/sounds/synth/sound_sequence.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> notes = { "Gb3", "Bb3", "Ab3", "C4", "Bb3" };
    SoundSequence soundSequence;
    for (auto& note: notes) {
        FourierWave* sound = new FourierWave(FourierSeriesAmplitudePhase(
            1.0/PitchData(note).frequency(), 
            {0, 0.5, 0.1, 0.09, 0.1, 0.05, 0.02}, 
            {0, 0, 0, 0, 0, 0, 0}
        ), 44100, 2, 44100);
        soundSequence << sound;
    }
    std::cout << soundSequence.sampleCount() << std::endl;
    SoundPlayer player(soundSequence);
    player.play();
    while(player.time() < player.duration()) {}
    return 0;
}

#endif 
