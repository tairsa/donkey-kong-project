#pragma once

#include "Ghost.h"

class SpecialGhost : public Ghost
{
public:
    // Constructor: Initializes SpecialGhost with 'X' as its character
    SpecialGhost(Board& board, int startX, int startY, bool silent)
        : Ghost(board, startX, startY, silent, 'X') {
    }

    void climbLadder() override;  // Handles climbing ladder logic

    // Checks if the ghost is above the floor
    bool isAboveFloor() const { return (ch_below == '=' || ch_below == '<' || ch_below == '>'); }

    // Checks if the ghost is underneath a floor
    bool isUnderneathFloor() const { return (ch_above == '=' || ch_above == '<' || ch_above == '>'); }

    bool isClimbingUp(int rand) const;  // Determines if the ghost should climb up
    void climbUp();                      // Moves the ghost up a ladder

    bool isClimbingDown(int rand) const; // Determines if the ghost should climb down
    void climbDown();                    // Moves the ghost down a ladder

    void walk();                           // Handles normal walking movement
    void checkState(int rand) ;       // Determines the ghost's current state (now const)
    void updateByState();                  // Updates the ghost's behavior based on its state
};

