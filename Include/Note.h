#ifndef NOTE_HPP
#define NOTE_HPP

#include <iostream>
#include <string>

enum class NoteType {
    NO_NOTE = 0,      // No note
    NORMAL_NOTE = 1,  // Normal tap note
    HOLD_HEAD = 2,    // Hold head 
    HOLD_TAIL = 3,    // Hold/Roll tail (end of a hold or roll)
    ROLL_HEAD = 4,    // Roll head 
    MINE = 'M'        // Mine 
};

class Note {
    public:
        int column;       // column index (0-3, 0 is left most arrow and 3 rightmost arrow) 
        NoteType type;    // type of the note 
        float startTime;  // time the note appears in seconds
        float endTime;    // for hold/roll notes, the time when it ends in seconds

        Note(int column, NoteType type, float startTime);

        Note(int column, NoteType type, float startTime, float endTime);

        void printNote() const;
};

#endif 
