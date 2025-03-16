#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>
#include "Note.h"

class Parser{
    public:
        Parser(const std::string& filepath);

        bool loadChart();

        const std::vector<Note>& getNotes() const { return notes; }

        const std::map<double, double>& getBpmChanges() const;

        double getOffset() const;
        
        double getNoteTimeForColumn(int col, double bpm);

        double getBPMAtTime(double time);

        void parseMeasures(const std::string& chartData);

    private: 
        std::string filepath;                 
        std::map<double, double> bpmChanges; // stores every bpm change, used for time calculations
        std::vector<Note> notes;             // stores every note in the loaded chart
        double offset;                       

        float getNoteTimeFromColumn(int col, double bpm);
};
#endif
 