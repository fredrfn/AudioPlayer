#include "sounds/processors/echo.hpp"
#include "sounds/processors/delay.hpp"

void Echo::processSamples(Sound* sound, [[maybe_unused]]SampleCount at, SoundBuffer& buffer) {
    if(_echoes == 0 || _spacing == 0 || _gain == 0.0) { 
        return; 
    }
    
    for(unsigned int echo = 0; echo < _echoes; echo++) {
        Delay(_spacing * echo, _echoes > 0 ? _gain*(1 - ((float)echo/_echoes)) : 0.0f).process(sound, at, buffer);
    }
}
