#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>

class Parser {
public:
    Parser(const std::string& filepath);

    bool loadChart();
    const std::map<std::string, std::vector<std::string>>& getDifficultyNotes() const;
    const std::string& getTitle() const;
    double getOffset() const;
    double getBPM() const;
    const std::map<double, double>& getBpmChanges() const;
private:
    std::string filepath;
    std::string title;
    double offset;
    double bpm;
    std::map<std::string, std::vector<std::string>> difficultyNotes;
    bool isValidNoteRow(const std::string& line) const;
    std::map<double, double> bpmChanges;
    std::string trim(const std::string& str);
    std::string convertNote(const std::string& line);
    std::vector<std::string> calculateTiming(const std::vector<std::string>& measure, int measureIndex, double bpm, double offset);
};

#endif // PARSER_H