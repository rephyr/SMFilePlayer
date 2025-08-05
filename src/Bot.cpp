#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>

void pressKey(WORD vk) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    input.ki.dwFlags = KEYEVENTF_SCANCODE; // scan code to try to bypass game anticheat
    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}
std::vector<WORD> noteToKeys(const std::string& note) {
    static const WORD keys[4] = {0x51, 0x57, 0x45, 0x52}; // Q, W, E, R
    std::vector<WORD> result;
    for (size_t i = 0; i < note.size() && i < 4; ++i) {
        if (note[i] == '1')
            result.push_back(keys[i]);
    }
    return result;
}

double getCurrentTime(const std::chrono::steady_clock::time_point& start) {
    using namespace std::chrono;
    return duration_cast<duration<double>>(steady_clock::now() - start).count();
}

// notesWithTimes: vector of pairs {note string, time in seconds}
void playChart(const std::vector<std::pair<std::string, double>>& notesWithTimes) {
    auto start = std::chrono::steady_clock::now();
    for (const auto& [note, noteTime] : notesWithTimes) {
        // quit the program by pressing ESC
        if (_kbhit() && _getch() == 27) { // 27 = ESC
            std::cout << "Bot stopped by user." << std::endl;
            break;
        }
        while (getCurrentTime(start) < noteTime) {
            if (_kbhit() && _getch() == 27) {
                std::cout << "Bot stopped by user." << std::endl;
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        for (WORD vk : noteToKeys(note)) {
            pressKey(vk);
        }
    }
}