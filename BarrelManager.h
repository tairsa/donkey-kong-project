#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Barrel.h"

class BarrelManager
{
	std::vector<Barrel> barrels;
	std::vector<Barrel> deadBarrels;  // Exploded barrels
	Board& board;
	int spawnCounter;
    long seed;
    bool isSilentMode;


public:

	BarrelManager(Board& gameBoard,bool silent) : board(gameBoard), spawnCounter(0),isSilentMode(silent) {}
    const std::vector<Barrel>& getBarrels() const; // Returns the list of active barrels
    const std::vector<Barrel>& getDeadBarrels() const; // Returns the list of exploded barrels

    void spawnBarrel(); // Spawns a new barrel at a random position
    void drawBarrels(); // Draws all active barrels on the screen
    void manageBarrelSpawning(); // Handles the timing logic for spawning barrels
    void updateBarrels(); // Updates the movement and state of all barrels
    void delDeadBarrels(); // Removes exploded barrels from the active list
    void eraseBarrels(); // Erases all barrels from the screen
    void clearBarrels(); // Clears all active barrels from the manager
    void clearDeadBarrels(); // Clears all exploded barrels from the manager
    void eraseHitBarrels(int targetX, int targetY);// Removes barrels located at the specified (targetX, targetY) position from the game.
    bool isBarrelAt(int x, int y) const;// Checks if there is a barrel at the given (x, y) position on the board.

};
