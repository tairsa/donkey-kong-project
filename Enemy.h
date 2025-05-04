#pragma once

#include "Point.h"
#include <Windows.h>
#include "utils.h"

class Enemy {
protected:
    Point enemy;                     // Position of the enemy on the game board.
    char ch_curr = ' ', ch_below = ' ', two_ch_below = ' ', ch_above = ' ', ch_left = ' ', ch_right = ' '; // Characters surrounding the enemy.
    char prevChar = ' ';             // Character at the enemy's previous position.
    GameCon::Direction dir{ 0, 0 };  // Current movement direction of the enemy.
    bool alive = true;               // Indicates whether the enemy is active.
    bool isSilentMode = false;

public:
    // Constructor: Initializes the enemy's position and associates it with the game board.
    Enemy(Board& board, int startX, int startY,bool silent) :enemy(startX, startY), isSilentMode(silent) {
        enemy.setBoard(board);
        
    }

    // Deleted default constructor to ensure proper initialization with a board and position.
    Enemy() = delete;

    // Returns the enemy's current x-coordinate.
    int getX() const { return enemy.getX(); }

    // Returns the enemy's current y-coordinate.
    int getY() const { return enemy.getY(); }

    // Erases the enemy from its current position on the game board.
    void eraseEnemy() {
        if(!isSilentMode)
             enemy.erase(); }

    // Returns the current movement direction of the enemy.
    GameCon::Direction getDirection() const { return dir; }

    // Returns the character directly below the enemy's current position.
    char getChBelow() const { return ch_below; }
};
