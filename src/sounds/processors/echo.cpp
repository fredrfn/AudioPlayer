#include "sounds/processors/echo.hpp"
#include "sounds/processors/delay.hpp"

void Echo::processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) {
    if(_echoes == 0 || _spacing == 0 || gains == nullptr) { 
        return; 
    }
    
    for(unsigned int echo = 0; echo < _echoes; echo++) {
        Delay(_spacing * echo, gains(echo)).process(sound, at, buffer);
    }
}
