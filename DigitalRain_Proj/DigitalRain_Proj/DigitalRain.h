<<<<<<< HEAD

/*
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
#ifndef DIGITALRAIN_H
#define DIGITALRAIN_H

#include <vector>   // For std::vector (dynamic arrays)
#include <memory>   // For std::unique_ptr (smart pointers)
#include <windows.h> // For Windows API console functions

/**
 * @brief Interface for the Digital Rain animation
 *
 * Uses Pimpl idiom to hide implementation details
 * Follows RAII principles for resource management
 */
class DigitalRain {
public:
    // Factory method to create instance (encapsulates implementation)
    static std::unique_ptr<DigitalRain> create(int width, int height);

    // Main animation loop controller
    virtual void run() = 0;

    // Virtual destructor for proper polymorphic deletion
    virtual ~DigitalRain() = default;

protected:
    // Protected constructor for implementation classes
    DigitalRain(int width, int height) : width(width), height(height) {}

    // Console dimensions (constant after construction)
    const int width;
    const int height;
};

#endif
=======
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
>>>>>>> cd8490e920fc444efb1ab6107204971f78b031a8
