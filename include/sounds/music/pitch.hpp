#ifndef SOUNDS_MUSIC_PITCH_HPP
#define SOUNDS_MUSIC_PITCH_HPP

#include <string>

#define A4_MIDI_NOTE_ON 69

typedef float Pitch;

enum class PitchClass { // value being the semitone spacing from C
    C = 0, 
    D = 2,
    E = 4, 
    F = 5, 
    G = 7,
    A = 9, 
    B = 11
};

enum class Accidental { 
    DOUBLE_FLAT = -2, 
    FLAT = -1, 
    NONE = 0, 
    SHARP = 1, 
    DOUBLE_SHARP = 2 
};

class PitchData {
    PitchClass _pitchClass;
    int _octava;
    Accidental _accidental;
    Pitch a4Temperament;
    Pitch _frequency;
    void computeFrequency();
public:
    PitchData(
        PitchClass pitchClass, 
        Accidental accidental = Accidental::NONE, 
        int octava = 4, 
        Pitch a4Temperament = 440
    );
    PitchData(std::string spn, Pitch a4Temperament = 440);
    std::string toString() const;
    char pitchChar() const;
    std::string alterationStr() const;
    PitchClass pitchClass() const { return _pitchClass; }
    unsigned int pitchClassValue() const { return static_cast<int>(_pitchClass); }
    int octava() const { return _octava; }
    Accidental accidental() const { return _accidental; }
    int accidentalValue() const { return static_cast<int>(_accidental); }
    Pitch frequency() const { return _frequency; }
    unsigned int midiNoteOnIndex() const;
};
#endif