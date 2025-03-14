#ifndef DIGITALRAIN_H
#define DIGITALRAIN_H

#include <vector>
#include <windows.h>

class DigitalRain {
public:
    DigitalRain(int width, int height);
    void run();

private:
    void update();
    void render();
    void setCursorPosition(int x, int y);

    int width, height;
    std::vector<std::vector<char>> grid;
    HANDLE consoleHandle;
};

#endif // DIGITALRAIN_H