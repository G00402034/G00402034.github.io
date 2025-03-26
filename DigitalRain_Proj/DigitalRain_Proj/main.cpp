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

#include "DigitalRain.h"
#include <windows.h> // For Windows console functions

/**
 * @brief Entry point for the Digital Rain application
 * Configures console and starts animation
 */
int main() {
    // Set console to UTF-8 mode
    system("chcp 65001 > nul");

    // Set initial console size
    system("mode con: cols=120 lines=40");

    // Get actual console window dimensions
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Create and run animation
    auto rain = DigitalRain::create(width, height);
    rain->run();

=======
#include "DigitalRain.h"

int main() {
    DigitalRain rain(118,30);
    rain.run();
>>>>>>> cd8490e920fc444efb1ab6107204971f78b031a8
    return 0;
}