#include <iostream>

#include "utils.h"

void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord; // Create a COORD structure 
    coord.X = x; // Set the X coordinate 
    coord.Y = y; // Set the Y coordinate 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
void setColor(int color) {
    // Get the standard output handle for the console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the color attribute of the console
    SetConsoleTextAttribute(hConsole, color);
}
void setColor(const std::string& color) {
    std::cout << color;
}