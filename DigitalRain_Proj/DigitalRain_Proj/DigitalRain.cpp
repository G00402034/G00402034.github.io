<<<<<<< HEAD
﻿/*
          c,_.--.,y
            7 a.a(
           (   ,_Y)
           :  '---;
       ___.'\.  - (
     .'"""S,._'--'_2..,_
     |    ':::::=:::::  \
     .     f== ;-,---.' T
      Y.   r,-,_/_      |
      |:\___.---' '---./
      |'`             )
       \             ,
       ':;,.________.;L
       /  '---------' |
       |              \
       L---'-,--.-'--,-'
        T    /   \   Y
        |   Y    ,   |
        |   \    (   |
        (   )     \,_L
        7-./      )  `,
       /  _(      '._  \
     '---'           '--'
     Sean Conroy
     G00402034
     15/03/2025
*/

#include "DigitalRain.h"
// Standard Library Headers
#include <iostream>   // For std::wcout (wide character output)
#include <random>     // For std::mt19937 (Mersenne Twister RNG)
#include <chrono>     // For std::chrono (precise timing)
#include <thread>     // For std::this_thread::sleep_for
#include <algorithm>  // For std::fill
// Windows-specific Headers
#include <io.h>       // For _setmode/_fileno (console mode setting)
#include <fcntl.h>    // For _O_U16TEXT (Unicode mode constant)

/**
 * Generates random characters for the rain effect
 * Uses only common ASCII characters as requested
 */
class CharacterGenerator {
    // Character set containing only specified symbols
    const std::vector<wchar_t> chars = {
        L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
        L'#', L'@', L'%', L'=', L'+', L'*', L':', L'-', L'.', L' '
    };
    std::mt19937 rng; // Mersenne Twister random number generator

public:
    CharacterGenerator() : rng(std::random_device{}()) {}

    wchar_t getRandomChar() {
        std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);
        return chars[dist(rng)];
    }
};

/**
 * Manages individual rain columns and their movement
 * Handles speed, position and reset logic
 */
class ColumnController {
    struct Column {
        int position; // Vertical position in console
        int speed;    // Pixels per frame
        int length;   // Length of the trail
    };

    std::vector<Column> columns; // All rain columns
    std::mt19937 rng;           // Random number generator

public:
    ColumnController(int count) : rng(std::random_device{}()) {
        columns.resize(count);
        resetAll();
    }

    void update() {
        for (auto& col : columns) {
            col.position += col.speed;
            if (col.position > 100) { // Reset if off-screen
                resetColumn(col);
            }
        }
    }

    void resetColumn(Column& col) {
        std::uniform_int_distribution<int> posDist(-50, -10);
        std::uniform_int_distribution<int> speedDist(1, 3);
        std::uniform_int_distribution<int> lenDist(5, 15);

        col.position = posDist(rng);
        col.speed = speedDist(rng);
        col.length = lenDist(rng);
    }

    void resetAll() {
        for (auto& col : columns) resetColumn(col);
    }

    const std::vector<Column>& getColumns() const { return columns; }
};

/**
 * Handles all console output operations
 * Uses double buffering to eliminate visible updates
 */
class RenderEngine {
    HANDLE console;          // Windows console handle
    std::wstring buffer;     // Pre-allocated output buffer
    std::wstring backBuffer; // Secondary buffer for swapping

public:
    RenderEngine(int width, int height) :
        buffer((width + 1)* height, L' '), // +1 for newline
        backBuffer((width + 1)* height, L' ') {

        // Get console handle and configure
        console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
        SetConsoleCursorInfo(console, &cursorInfo);

        // Set UTF-8 and wide character mode
        SetConsoleOutputCP(CP_UTF8);
        _setmode(_fileno(stdout), _O_U16TEXT);
    }

    /**
     *  Renders a frame with no visible updates
     *  frame The 2D vector of characters to render
     */
    void render(const std::vector<std::vector<wchar_t>>& frame) {
        // Build frame in back buffer
        size_t idx = 0;
        for (const auto& row : frame) {
            for (wchar_t c : row) {
                backBuffer[idx++] = c;
            }
            backBuffer[idx++] = L'\n';
        }

        // Atomic swap and output
        std::swap(buffer, backBuffer);

        // Single console write operation
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        SetConsoleCursorPosition(console, { 0, 0 });
        std::wcout.write(buffer.data(), buffer.size());
        std::wcout.flush();
    }
};

/**
 *  Implementation of DigitalRain interface
 * Coordinates all components for the animation
 */
class DigitalRainImpl : public DigitalRain {
    std::unique_ptr<ColumnController> columns;
    std::unique_ptr<RenderEngine> renderer;
    std::unique_ptr<CharacterGenerator> charGen;
    std::vector<std::vector<wchar_t>> currentFrame;
    std::vector<std::vector<wchar_t>> nextFrame;

public:
    DigitalRainImpl(int width, int height) : DigitalRain(width, height),
        currentFrame(height, std::vector<wchar_t>(width, L' ')),
        nextFrame(height, std::vector<wchar_t>(width, L' ')) {

        // Initialize components
        columns = std::make_unique<ColumnController>(width);
        renderer = std::make_unique<RenderEngine>(width, height);
        charGen = std::make_unique<CharacterGenerator>();
    }

    void run() override {
        using namespace std::chrono;
        auto lastFrame = steady_clock::now();
        constexpr auto frameTime = 16ms; // Target 60 FPS

        while (true) {
            auto now = steady_clock::now();

            // Fixed timestep update
            if (now - lastFrame >= frameTime) {
                update();
                render();
                lastFrame = now;

                // Precise sleep to maintain frame rate
                auto remaining = frameTime - (steady_clock::now() - now);
                if (remaining > 0ms) {
                    std::this_thread::sleep_for(remaining);
                }
            }
        }
    }

private:
    void update() {
        columns->update();

        // Clear next frame
        for (auto& row : nextFrame) {
            std::fill(row.begin(), row.end(), L' ');
        }

        // Update all columns
        const auto& cols = columns->getColumns();
        for (int j = 0; j < width; ++j) {
            const auto& col = cols[j];
            for (int i = 0; i < height; ++i) {
                if (i >= col.position && i < col.position + col.length) {
                    nextFrame[i][j] = charGen->getRandomChar();
                }
            }
        }
    }

    void render() {
        renderer->render(nextFrame);
        currentFrame.swap(nextFrame); // Swap buffers
    }
};

// Factory implementation
std::unique_ptr<DigitalRain> DigitalRain::create(int width, int height) {
    return std::make_unique<DigitalRainImpl>(width, height);
=======
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
>>>>>>> cd8490e920fc444efb1ab6107204971f78b031a8
}