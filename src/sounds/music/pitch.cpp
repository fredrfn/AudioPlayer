#include "sounds/music/pitch.hpp"

#define PITCH_SEMITONE_FACTOR 1.05946309436

#include <cmath>
#include <regex>
#include <iostream>

PitchData::PitchData(PitchClass pitchClass, Accidental accidental, int octava, Pitch a4Temperament) : 
    _pitchClass(pitchClass), _octava(octava), _accidental(accidental), a4Temperament(a4Temperament) {
    computeFrequency();
}

PitchData::PitchData(std::string spn, Pitch a4Temperament) : a4Temperament(a4Temperament) {
    _pitchClass = PitchClass::A;
    _octava = 4;
    _accidental = Accidental::NONE;
    _frequency = a4Temperament;

    const std::regex spnRegex("([A-G])([#b]{0,2})([-]?)([0-9]?)");
    std::smatch matches;

    if (std::regex_match(spn, matches, spnRegex)) {
        for (size_t i = 1; i < matches.size(); ++i) {
            std::ssub_match sub_match = matches[i];
            std::string match = sub_match.str();
            switch (i - 1) {
                case 0: {
                    switch(match[0]) {
                        case 'A': _pitchClass = PitchClass::A; break;
                        case 'B': _pitchClass = PitchClass::B; break;
                        case 'C': _pitchClass = PitchClass::C; break;
                        case 'D': _pitchClass = PitchClass::D; break;
                        case 'E': _pitchClass = PitchClass::E; break;
                        case 'F': _pitchClass = PitchClass::F; break;
                        case 'G': _pitchClass = PitchClass::G; break;
                        default: return;
                    }; 
                    break;
                }
                case 1: {
                    int multiplier = 0;
                    if (match.length() > 0) {
                        switch(match[0]) {
                            case '#': multiplier = 1; break;
                            case 'b': multiplier = -1; break;
                            default: return;
                        }
                        _accidental = static_cast<Accidental>(multiplier * match.length());
                    }
                    break;
                }
                case 2:
                    _octava = match.length() > 0 && match[0] == '-' ? -1 : 1;
                    break;
                case 3:
                    _octava = _octava * (match.length() > 0 ? match[0] - '0' : 4);
                    break;
                default: break;
            }
        }   
    } else {
        std::cout << "Pitch SPN did not match regex!" << std::endl;
    }
    computeFrequency();
}

void PitchData::computeFrequency() { 
    int exponent = midiNoteOnIndex() - 69;
    _frequency = a4Temperament * (float)pow(PITCH_SEMITONE_FACTOR, exponent); 
}

std::string PitchData::toString() const { 
    return "" + pitchChar() + alterationStr() + std::to_string(_octava); 
}

char PitchData::pitchChar() const { 
    return 'A' + ((int)ceilf(pitchClassValue() + static_cast<int>(PitchClass::A) / 2.0f)) % 7; 
}

std::string PitchData::alterationStr() const {
    auto value = accidentalValue();
    return accidentalValue() == 0 ? "" : std::string(abs(value), value < 0 ? 'b' : '#');
}

unsigned int PitchData::midiNoteOnIndex() const { 
    return 12 * (_octava + 1) + pitchClassValue() + accidentalValue(); 
}
