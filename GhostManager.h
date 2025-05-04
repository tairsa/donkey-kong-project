#pragma once

#include "Ghost.h";
#include "SpecialGhost.h"; 

class GhostManager
{
    std::vector<Ghost*> ghosts;  // List of pointers to ghosts
    Board& board;               // Reference to the game board.

public:
    // Constructor: Initializes the GhostManager with a reference to the game board.
    GhostManager(Board& board) : board(board) {}

    // Returns the list of active ghosts in the game.
    const std::vector<Ghost*>& getGhosts() const { return ghosts; }  // Updated to return vector of pointers

    // Adds a new ghost to the game at the specified starting position.
    void addGhost(int startX, int startY, bool isSpecial, bool silent);

    // Handles collisions between ghosts in the game.
    void handleCollision(std::vector<Ghost*>::iterator it);

    // Updates the state and position of all ghosts.
    void updateGhosts();

    // Erases all ghosts from the game board without clearing the list.
    void eraseGhosts();

    // Clears all ghosts from the game board and removes them from the list.
    void clearGhosts();

    void drawGhosts() {
        for (Ghost* ghost : ghosts) {
            ghost->drawGhost(); // Draw each ghost
        }
    }

    // Removes a ghost located at the specified (targetX, targetY) position.
    void eraseHitGhosts(int targetX, int targetY);

    // Checks if there is a ghost at the specified (x, y) position on the game board.
    bool isGhostAt(int x, int y) const;
    ~GhostManager();
};