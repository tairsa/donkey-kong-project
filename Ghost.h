#pragma once

#include "Point.h"
#include "Enemy.h"

class Ghost : public Enemy
{
protected:
    char GHOST;      // Character used to represent the barrel
    char ch_left_below = ' ', ch_right_below = ' '; // Characters surrounding the ghost
    int diedX = -1, diedY = -1;    // Coordinates where the ghost died
    enum State { movingL, movingR, diedFromHammer, born };
    State currentState = born;              // Current state of the ghost
    bool isDuringCollision = false;
    bool OnLadder = false;
    // Enumeration for Mario's possible states
    enum class GhostState {
        ClimbingUP,         // Mario is climbing up a ladder
        ClimbingDown,       // Mario is climbing down a ladder
        Walking // Mario is walking or standing still
    };
    GhostState state = GhostState::Walking; // Current state of Mario

public:
    Ghost(Board& board, int startX, int startY, bool silent, char ghost = 'x') : Enemy(board, startX, startY,silent), GHOST(ghost)
    {
        /*myGhost.setBoard(board);*/
        dir = { 1 , 0 };
        // Remove the 'x' from the text file by replacing it with a space
        enemy.getBoard()->setChar(startX, startY, ' '); // Replace 'x' with a space
        ch_below = enemy.getBoard()->getChar(startX, startY + 1);
    }

    void setIsDuringCollision(bool curr) { isDuringCollision = curr; }// Sets whether the ghost is currently involved in a collision.

    void updateDirection();// Updates the ghost's movement direction based on its surroundings and behavior.

    void reverseDirection();// Reverses the ghost's current movement direction.

    bool checkCollisionWithOtherGhost(const Ghost* otherGhost) const;// Checks if the ghost has collided with another ghost at the same position.

    void move();// Moves the ghost according to its current direction and game logic.

    void drawGhost();// Draws the ghost on the game board.

    virtual void climbLadder() {};

    bool isOnLadder() const
    {
        return (ch_curr == 'H' || two_ch_below == 'H');	// Returns true only if mario is on the letter 'H' or above it
    }

};