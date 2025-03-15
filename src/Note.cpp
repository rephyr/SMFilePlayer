#include "../Include/Note.h"

// constructor for a normal note
Note::Note(int column, NoteType type, float startTime)
    : column(column), type(type), startTime(startTime), endTime(0) {}

// constructor for hold or roll notes 
Note::Note(int column, NoteType type, float startTime, float endTime)
    : column(column), type(type), startTime(startTime), endTime(endTime) {}

// for debugging purposes
void Note::printNote() const {
    std::cout << "Column: " << column << ", Type: " << static_cast<int>(type)
              << ", Start Time: " << startTime << ", End Time: " << endTime << std::endl;
}
