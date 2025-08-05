#ifndef BOT_H
#define BOT_H

#include <windows.h>
#include <vector>
#include <string>
#include <chrono>

void pressKey(WORD vk);
std::vector<WORD> noteToKeys(const std::string& note);
double getCurrentTime(const std::chrono::steady_clock::time_point& start);
void playChart(const std::vector<std::pair<std::string, double>>& notesWithTimes);

#endif