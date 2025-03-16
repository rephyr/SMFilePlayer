#include "../Include/Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Parser::Parser(const std::string& filepath) : filepath(filepath), offset(0.0) {
    if (!loadChart()) {
        std::cerr << "Failed to load chart: " << filepath << std::endl;
    }
}

bool Parser::loadChart() {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filepath << std::endl;
        return false;
    }

    std::string chartData;
    std::string line;
    bool notesSectionFound = false;

    while (std::getline(file, line)) {
        if (line.find("#NOTES:") != std::string::npos) {
            notesSectionFound = true;
            continue;  
        }

        if (!notesSectionFound) {
            continue;
        }

        // skip metadata lines 
        if (line.empty() || line.find_first_not_of("01234M") != std::string::npos) {
            continue;  // Skip empty lines and non-step data lines
        }

        chartData += line + "\n";
    }

    file.close();  

    // debugging
    if (!notesSectionFound) {
        std::cerr << "Error: No valid #NOTES section found in " << filepath << std::endl;
        return false;
    }

    parseMeasures(chartData); 
    return true;
}

void Parser::parseMeasures(const std::string& chartData) {
    std::istringstream stream(chartData);
    std::string measure;
    int measureIndex = 0;

    while (std::getline(stream, measure, ',')) {  
        std::istringstream measureStream(measure);
        std::string line;
        std::vector<std::string> rows;
        
        while (std::getline(measureStream, line)) {
            if (!line.empty()) {
                rows.push_back(line);
            }
        }

        int rowsPerMeasure = rows.size();  
        if (rowsPerMeasure == 0) continue; 

        double bpm = getBPMAtTime(measureIndex * 4.0);
        double measureDuration = 4.0 * 60.0 / bpm;  
        double timePerRow = measureDuration / rowsPerMeasure;

        for (int rowIndex = 0; rowIndex < rowsPerMeasure; ++rowIndex) {
            const std::string& row = rows[rowIndex];
            double noteTime = measureIndex * measureDuration + (rowIndex * timePerRow) + offset;

            // Ensure we're only reading 4 characters (columns)
            for (size_t col = 0; col < row.length() && col < 4; ++col) {
                char noteChar = row[col];
                NoteType type = NoteType::NO_NOTE;

                switch (noteChar) {
                    case '0': type = NoteType::NO_NOTE; break;
                    case '1': type = NoteType::NORMAL_NOTE; break;
                    case '2': type = NoteType::HOLD_HEAD; break;
                    case '3': type = NoteType::HOLD_TAIL; break;
                    case '4': type = NoteType::ROLL_HEAD; break;
                    case 'M': type = NoteType::MINE; break;
                }

                if (type != NoteType::NO_NOTE) {
                    if (type == NoteType::HOLD_HEAD || type == NoteType::ROLL_HEAD) {
                        // FIXME: HOLD TIMES
                        notes.push_back(Note(col, type, noteTime, noteTime + 2.0)); 
                    } else {
                        notes.push_back(Note(col, type, noteTime));
                    }
                }
            }
        }

        measureIndex++;
    }
}


double Parser::getBPMAtTime(double time){
    auto it = bpmChanges.lower_bound(time);
    if (it != bpmChanges.end()) {
        return it->second;
    }
    return -1.0;  // default BPM if no changes are found
}

float Parser::getNoteTimeFromColumn(int col, double bpm) {
    // for now, we assume each note is spaced out by 1/4 second in terms of columns
    float noteTime = static_cast<float>((col) / bpm);  
    return noteTime + static_cast<float>(offset);
}

const std::map<double, double>& Parser::getBpmChanges() const {
    return bpmChanges;
}

double Parser::getOffset() const {
    return offset;
}