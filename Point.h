#pragma once
#include <iostream>

#include "utils.h"
#include "Board.h"
#include "GameCon.h"

/**
 * Represents a point on the game board.
 * Handles position, rendering, and interactions with the board.
 */
class Point
{
    int x, y;                           // Coordinates of the point on the board
    Board* pBoard = nullptr;            // Pointer to the board this point belongs to
    char underlyingChar = ' ';          // Character under the current position

public:
    Point(int _x, int _y) : x(_x),y(_y){};
    void setXY(int _x, int _y);         // Sets the point's coordinates
    int getX() const { return x; }      // Gets the x-coordinate
    int getY() const { return y; }      // Gets the y-coordinate
    void setUnderlyingChar(char c)  { underlyingChar = c; } // Sets the underlying character
    char getUnderlyingChar() const { return underlyingChar; }    // Gets the underlying character

    Board* getBoard() const { return pBoard; }             // Gets the board the point belongs to
    void draw(char c) const {                              // Draws a character at the point
        gotoxy(x, y);
        std::cout << c;
        std::cout.flush(); // Ensure output is flushed immediately
    }
    void erase() {                                         // Erases the point by restoring the underlying character
        draw(pBoard->getChar(x, y));
    }
    void setBoard(Board& board) { pBoard = &board; }       // Sets the board reference
};
