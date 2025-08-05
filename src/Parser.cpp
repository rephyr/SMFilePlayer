#include "../Include/Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <cmath>
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>

Parser::Parser(const std::string& filepath) : filepath(filepath), offset(0.0), bpm(120.0) {
    if (!loadChart()) {
        std::cerr << "Failed to load chart: " << filepath << std::endl;
    }
}

std::string Parser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::string Parser::convertNote(const std::string& line) {
    return std::regex_replace(std::regex_replace(line, std::regex("[MKLF]"), "0"), std::regex("4"), "1");
}

bool Parser::loadChart() {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filepath << std::endl;
        return false;
    }

    std::string line, currentDifficulty;
    std::vector<std::string> measure;
    int measureIndex = 0;
    bool readNotes = false;
    std::string readValues;
    double sampleLength = 0.0; 

    while (std::getline(file, line)) {
        line = trim(line);

        if (!readNotes) {
            if (line.find("#NOTES:") == 0) {
                readNotes = true;
            } else {
                readValues += line;
                if (readValues.back() == ';') {
                    auto splitPos = readValues.find(":");
                    std::string dataName = readValues.substr(1, splitPos - 1);
                    std::string dataValue = readValues.substr(splitPos + 1, readValues.size() - splitPos - 2);

                    if (dataName == "TITLE") {
                        title = dataValue;
                    } else if (dataName == "BPMS") {
                        if (dataValue.find(",") != std::string::npos) {
                            throw std::runtime_error("Multiple BPMs detected");
                        }
                        bpm = std::stod(dataValue.substr(dataValue.find("=") + 1));
                    } else if (dataName == "STOPS" && !dataValue.empty()) {
                        throw std::runtime_error("Stop detected");
                    } else if (dataName == "OFFSET") {
                        offset = std::stod(dataValue);
                    } else if (dataName == "SAMPLELENGTH") {
                        sampleLength = std::stod(dataValue);
                        std::cout << "SAMPLELENGTH: " << sampleLength << " seconds" << std::endl;
                    }
                    readValues.clear();
                }
            }
        } else {
            if (line.find("#NOTES:") == 0) {
                measureIndex = 0;
                std::getline(file, line); // skip 3 lines to reach difficulty
                std::getline(file, line);
                std::getline(file, currentDifficulty);
            } else if (line.find(",") == 0 || line.find(";") == 0) {
                // calculate the timings of the notes
                auto notesAndTimings = calculateTiming(measure, measureIndex, bpm, offset + sampleLength);
                difficultyNotes[currentDifficulty].insert(difficultyNotes[currentDifficulty].end(), notesAndTimings.begin(), notesAndTimings.end());
                measure.clear();
                ++measureIndex;
            } else if (!line.empty()) {
                if (isValidNoteRow(line)) {
                    std::string note = convertNote(line);
                    measure.push_back(note);
                }
            }
        }
    }

    file.close();
    return true;
}

std::vector<std::string> Parser::calculateTiming(const std::vector<std::string>& measure, int measureIndex, double bpm, double offset) {
    double measureSeconds = 4 * 60 / bpm; // one measure in seconds
    double measureTiming = measureSeconds * measureIndex; // try to time current measure

    std::vector<std::string> noteAndTimings;
    if (measure.empty()) return noteAndTimings;
    double rowDuration = measureSeconds / measure.size();

    for (size_t i = 0; i < measure.size(); ++i) {
        if (!measure[i].empty()) {
            double noteTiming = i * rowDuration + measureTiming + offset;
            noteAndTimings.push_back(measure[i] + " " + std::to_string(noteTiming));
        }
    }
    return noteAndTimings;
}

bool Parser::isValidNoteRow(const std::string& line) const {
    return std::all_of(line.begin(), line.end(), [](char c) {
        return c == '0' || c == '1' || c == '2' || c == '3' || c == '4';
    });
}

const std::map<std::string, std::vector<std::string>>& Parser::getDifficultyNotes() const {
    return difficultyNotes;
}

const std::string& Parser::getTitle() const {
    return title;
}

double Parser::getOffset() const {
    return offset;
}

double Parser::getBPM() const {
    return bpm;
}