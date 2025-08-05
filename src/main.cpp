#include "../Include/Parser.h"
#include "../Include/Bot.h" 
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>

int main() {
    std::string chartPath = "E:/EtternaBot/Charts/asd.sm";
    Parser parser(chartPath);

    // collect notes and timings
    std::vector<std::pair<std::string, double>> notesWithTimes;
    const auto& difficultyNotes = parser.getDifficultyNotes();
    for (const auto& [difficulty, notes] : difficultyNotes) {
        for (const auto& noteLine : notes) {
            std::istringstream iss(noteLine);
            std::string note;
            double time;
            iss >> note >> time;
            if (note != "0000")
                notesWithTimes.emplace_back(note, time);
        }
    }

    // we use f8 as a key to start the chart (press enter) and
    // start the bot at the same time. After pressing f8
    // the program waits a few seconds so you can focus the game window
    // and presses enter and start the bot at the same time.
    if (!RegisterHotKey(NULL, 1, 0, VK_F8)) {
        std::cerr << "Hotkey register fail" << std::endl;
        return 1;
    }
    std::cout << "Press F8 to start the bot." << std::endl;

    // Wait for F8
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            std::cout << "F8 pressed! Starting in 2 seconds..." << std::endl;
            break;
        }
    }
    UnregisterHotKey(NULL, 1);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    pressKey(VK_RETURN);
    playChart(notesWithTimes);

    std::cout << "Done" << std::endl;
    return 0;
}