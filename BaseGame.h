#pragma once

#include <iostream>        //for cout
#include <conio.h>        //for _kbhit(), _getch()
#include <Windows.h>    //for Sleep and colors
#include "utils.h"

#include "Mario.h"
#include "BarrelManager.h"
#include "GhostManager.h" 
#include "Steps.h"
#include "Results.h"

class BaseGame
{
protected:
    static constexpr int DEAD_MARIO = 0;   // Constant indicating Mario's death (0 lives).
    static constexpr int FULL_LIVES = 3;   // Maximum lives Mario can have.
    static constexpr int EXPLOSION_RADIUS = 2; // Radius for barrel explosion effects.
    static bool colorMode;                 // Flag for enabling/disabling color mode.
    bool playing_mario = true;             // True while Mario has more than 0 lives.
    bool exit_game = false;                // True when the player chooses to exit the game.

    static const int SCORE_PER_ENEMY = 10; // Points awarded for defeating an enemy.
    int score = 0;                         // Current game score.
    bool shouldUpdateLegend = false;       // Flag for updating the game legend display.

    Board board;                           // The game board object.
    Mario mario;                           // The Mario player object.
    BarrelManager barrelManager;           // Manager for barrels in the game.
    GhostManager ghostManager;             // Manager for ghosts in the game.

    Steps steps;
    size_t iteration = 0;
    long random_seed;
    Results results;
    bool currIterationLostLife = false;
    bool isSilentMode;

    std::vector<std::string> files_names_vec; // List of available game level files.

public:

    // Constructor: Initializes the game with board, Mario, and managers.
    BaseGame(bool silent=false) :mario(board,silent), barrelManager(board,silent), ghostManager(board) {
        isSilentMode = silent;
    }

    // Runs the main game loop.
    virtual void run()=0;

    // Starts the game for the selected level.
    virtual void startGame(int screen_index) = 0;

    // Handles game-over logic when the player loses.
    virtual void loose()=0;


    // Updates Mario's actions based on key presses from the user.
    virtual void updateAction(int key)=0;

    // Restarts the current game level.
    void restartLevel();

    // Checks if Mario is touching a barrel.
    bool isTouchingBarrel() const;

    // Checks if Mario successfully completed the level.
    bool wonTheLevel();

    // Updates the game state if Mario is hit or affected by a barrel.
    void updateIfDiedByBarrel();

    // Updates the game state if Mario is hit by a ghost.
    void updateIfDiedByGhost();

    // Checks if Mario died due to a barrel explosion at a specific position.
    void diedFromExplodedBarrel(int barrelPosX, int barrelPosY, int marioPosX, int marioPosY, int i);

    // Checks if Mario fell from a height that is too high.
    void checkIfMarioFellFromHigh();

    // Flashes Mario on the screen (e.g., when respawning or taking damage).
    void flashMario();

    // Initializes ghosts at their starting positions.
    void initializeGhosts();

    // Checks if Mario is collecting the hammer power-up.
    void isCollectingHammer();

    // Checks if Mario is using the hammer to defeat enemies.
    void checkAndUseHammer();

    // Handles hammer usage to attack barrels or ghosts.
    void useHammer(int marioX, int marioY, const GameCon::Direction& dir);

    // Handles logic when a hammer hits a barrel.
    void handleHammerHitBarrel(int targetX, int targetY);

    // Handles logic when a hammer hits a ghost.
    void handleHammerHitGhost(int targetX, int targetY);

    // Updates the game legend (score, lives, etc.).
    void updateLegendState();

    // Checks if Mario is off the limits of the game board.
    void checkIfOffLimits();

    // Resets the game score and updates the legend display.
    void resetScore() { score = 0; shouldUpdateLegend = true; }

    // Adds points to the score and updates the legend display.
    void addScore() { score += SCORE_PER_ENEMY; shouldUpdateLegend = true; }
};