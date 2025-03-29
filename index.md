---
title: "Digital Rain Simulation in C++"
date: 2025-03-15
author: "Sean Conroy"
---

## Digital Rain Simulation in C++
![Digital Rain in Action](assets/VsDebugConsole_NiEamvhBHi.gif)
## UML Diagram of Digital Rain Project
![UML Diagram](assets/chrome_wlvICTHz0N.png)
The diagram shows the main classes and their relationships

Private members are marked with "-", public with "+", protected with "#"

Static methods are marked with {static}

Abstract/virtual methods are marked appropriately

The Column struct is shown as a separate class since it has its own behavior

Windows API dependencies are not shown in the diagram as they're implementation details
### Introduction

This project is a C++ implementation of the classic "Digital Rain" effect inspired by the Matrix movies. It utilizes modern C++ techniques, including object-oriented programming, smart pointers, and Windows API functions for console manipulation. The goal was to create an efficient, visually appealing, and optimized implementation of the cascading text effect in a Windows console environment.

### Design & Test

The design consists of a few core components:
- **Character Generator**: A class that randomly selects characters to be displayed in the rain effect.
- **Column Controller**: Manages the individual columns of text, their speed, length, and reset behavior.
- **DigitalRain Class**: The main class that integrates all components and manages the rendering process.

To test the simulation, the program was run in different console sizes, ensuring that characters flowed smoothly, and performance remained stable. Edge cases such as screen resizing and excessive CPU usage were also considered.

### Algorithm

1. Initialize the console window with the required dimensions and encoding.
2. Create and manage multiple columns of cascading characters.
3. Randomly determine character selection, movement speed, and length for each column.
4. Continuously update and redraw the console output to simulate falling characters.
5. Ensure columns reset properly once they exit the screen.

This algorithm ensures smooth and non-repetitive animation while keeping computational overhead minimal.

### Problem-solving

Several challenges arose during development:
- **Character Encoding Issues**: The console needed to be set to UTF-8 mode to properly display Unicode characters.
- **Randomization Optimization**: Ensuring a fair distribution of characters without noticeable repetition.
- **Performance Optimization**: Reducing flickering and CPU overhead by efficiently managing screen refresh rates and character updates.
- **Column Movement Synchronization**: Columns had to move at different speeds without disrupting the overall effect.

Each of these issues was addressed through research, testing, and iterative improvements to the codebase.

### Modern C++ Insight & Reflection

This project was a great opportunity to leverage modern C++ features, including:
- **Smart Pointers**: Used to manage memory dynamically without manual deallocation.
- **Random Number Generation**: Implemented using the `<random>` library for realistic effects.
- **Object-Oriented Design**: Ensured modularity and reusability of components.
- **Multi-threading Considerations**: While not implemented, future improvements could include multi-threading for enhanced performance.

Through this project, I gained valuable insights into efficient console rendering, working with Windows API, and structuring code for clarity and maintainability.

### How to Run the Code

### Prerequisites
- A **Windows** system with a command-line terminal.
- A C++ compiler (e.g., `g++` from MinGW or MSVC).

### Compilation & Execution
1. Download the repository.
2. Open your terminal and navigate to the project directory.
3. Compile the code:
```sh
g++ -o digital_rain DigitalRain.cpp Main.cpp -std=c++17 -Wall
Run the compiled executable:

sh
Copy
Edit
digital_rain.exe
For Linux/macOS, you may need to replace windows.h with cross-platform alternatives.

Code Implementation
DigitalRain.h
cpp
Copy
Edit
#ifndef DIGITALRAIN_H
#define DIGITALRAIN_H

#include <vector>
#include <memory>
#include <windows.h>

class DigitalRain {
public:
    static std::unique_ptr<DigitalRain> create(int width, int height);
    virtual void run() = 0;
    virtual ~DigitalRain() = default;

protected:
    DigitalRain(int width, int height) : width(width), height(height) {}
    const int width;
    const int height;
};

#endif
DigitalRain.cpp
cpp
Copy
Edit
#include "DigitalRain.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <io.h>
#include <fcntl.h>

class CharacterGenerator {
    const std::vector<wchar_t> chars = {
        L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
        L'#', L'@', L'%', L'=', L'+', L'*', L':', L'-', L'.', L' '
    };
    std::mt19937 rng;

public:
    CharacterGenerator() : rng(std::random_device{}()) {}
    wchar_t getRandomChar() {
        std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);
        return chars[dist(rng)];
    }
};

class ColumnController {
    struct Column {
        int position;
        int speed;
        int length;
    };

    std::vector<Column> columns;
    std::mt19937 rng;

public:
    ColumnController(int count) : rng(std::random_device{}()) {
        columns.resize(count);
        resetAll();
    }

    void update() {
        for (auto& col : columns) {
            col.position += col.speed;
            if (col.position > 100) {
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
Main.cpp
cpp
Copy
Edit
#include "DigitalRain.h"
#include <windows.h>

int main() {
    system("chcp 65001 > nul");
    system("mode con: cols=120 lines=40");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    auto rain = DigitalRain::create(width, height);
    rain->run();

    return 0;
}
Conclusion
This Digital Rain simulation effectively demonstrates advanced C++ features, random generation, and efficient console rendering. It provides a visually engaging representation of cascading characters, bringing the iconic Matrix aesthetic to life in the console. Future enhancements could include multi-threading, color variations, and even graphical implementations using a GUI framework.
