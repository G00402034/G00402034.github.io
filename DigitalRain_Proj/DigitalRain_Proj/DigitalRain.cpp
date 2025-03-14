#include "DigitalRain.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

DigitalRain::DigitalRain(int width, int height) : width(width), height(height) {
    std::srand(std::time(0));
    grid.resize(height, std::vector<char>(width, ' '));
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void DigitalRain::run() {
    while (true) {
        update();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void DigitalRain::update() {
    for (int i = height - 1; i > 0; --i) {
        grid[i] = grid[i - 1];
    }
    for (int j = 0; j < width; ++j) {
        grid[0][j] = (std::rand() % 2) ? static_cast<char>(std::rand() % 94 + 33) : ' ';
    }
}

void DigitalRain::render() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            setCursorPosition(j, i);
            std::cout << grid[i][j];
        }
    }
}

void DigitalRain::setCursorPosition(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(consoleHandle, coord);
}