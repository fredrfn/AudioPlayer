#include "sounds/music/pitch.hpp"

#include <cmath>
#include <regex>

PitchData::PitchData(PitchClass pitchClass, Accidental accidental = Accidental::NONE, int octava = 4, Pitch a4Temperament = 440) : 
    _pitchClass(pitchClass), _octava(octava), _accidental(accidental), a4Temperament(a4Temperament) {
    computeFrequency();
}

PitchData::PitchData(const std::string& spn, Pitch a4Temperament = 440) : a4Temperament(a4Temperament) {
    _pitchClass = PitchClass::A;
    _octava = 4;
    _accidental = Accidental::NONE;
    _frequency = a4Temperament;

    const std::regex spnRegex("([A-G])([#b]{0,2})([-]?)([0-9]?)");
    std::smatch matches;

    if (std::regex_match(spn, matches, spnRegex)) {
        for (size_t i = 0; i < matches.size(); ++i) {
            std::ssub_match sub_match = matches[i];
            std::string match = sub_match.str();
            switch (i) {
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
    }
    computeFrequency();
}

void PitchData::computeFrequency() { 
    _frequency = a4Temperament * powf(2.0f, midiNoteOnIndex() - A4_MIDI_NOTE_ON/12.0f); 
}

std::string PitchData::toString() const { 
    return "" + pitchChar() + alterationStr() + std::to_string(_octava); 
}

char PitchData::pitchChar() const { 
    return 'A' + ((int)ceilf(pitchClassValue() + static_cast<int>(PitchClass::A) / 2.0f)) % 7; 
}

std::string PitchData::alterationStr() const {
    auto value = accidentalValue();
    return accidentalValue == 0 ? "" : std::string(abs(value), value < 0 ? 'b' : '#');
}

unsigned int PitchData::midiNoteOnIndex() const { 
    return 12 * (_octava + 1) + pitchClassValue() + accidentalValue(); 
}
