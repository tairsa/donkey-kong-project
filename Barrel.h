#pragma once

#include "Point.h"
#include <Windows.h>
#include "utils.h"
#include "Enemy.h"

/**
 * Represents a single barrel in the game.
 * Handles movement, state transitions, explosions, and rendering.
 */
class Barrel: public Enemy {
private:
    static constexpr char BARREL = 'O';      // Character used to represent the barrel
    static constexpr int HIGH_FALL = 8;      // Maximum fall distance before the barrel explodes
    int fallDistance = 0;                    // Distance the barrel has fallen
    int explosionX = -1, explosionY = -1;    // Coordinates where the barrel exploded

public:
    Barrel(Board& board, int startX, int startY,bool silent):Enemy(board,startX,startY,silent) {}

    /**
     * Represents the possible states of a barrel.
     * - rollingL: Rolling to the left.
     * - rollingR: Rolling to the right.
     * - falling: Falling down.
     * - exploding: Exploding after a high fall or collision.
     * - born: Initial state of the barrel.
     */
    enum State { rollingL, rollingR, falling, exploding, born };
    State currentState = born;              // Current state of the barrel
    State prevStateBeforeFalling;           // Previous state before the barrel started falling

    // Methods
    void move();                            // Updates the barrel's position and checks its state
    void checkState();                      // Determines the barrel's next state based on its surroundings
    void rollRight();                       // Sets the barrel to roll right
    void rollLeft();                        // Sets the barrel to roll left
    void fall();                            // Handles the barrel falling down
    void explode();                         // Executes the barrel's explosion logic
    void updateByState();                   // Updates the barrel's behavior based on its current state
    void updateNextMove();                  // Calculates and updates the barrel's next position
    bool isFalling() const ;                       // Checks if the barrel is falling
    bool isExplode() const ;                       // Checks if the barrel should explode
    bool isRollingLeft() const;                   // Checks if the barrel is rolling left
    bool isRollingRight() const;                  // Checks if the barrel is rolling right
    void drawBarrel();                      // Draws the barrel on the screen
    State getState() const { return currentState; } // Returns the barrel's current state
    bool getAlive() const { return alive; } // Checks if the barrel is active
    int getExplosionX() const { return explosionX; } // Gets the x-coordinate of the barrel's explosion
    int getExplosionY() const { return explosionY; } // Gets the y-coordinate of the barrel's explosion
    bool isCharBelowLimit() const { return ch_below == 'Q'; }// Checks if the character below the object is the 'Q' character, indicating a specific boundary or limit.

 
};