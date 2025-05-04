#pragma once

#include "BaseGame.h"

class HumanInputGame:public BaseGame
{
    bool isSaveMode;
  
public:
    // Returns the current state of the color mode (enabled or disabled).
    static bool getColorMode() { return colorMode; }

    // Constructor: Initializes the game with board, Mario, and managers.
    HumanInputGame(bool is_Save_Mode) :BaseGame(), isSaveMode(is_Save_Mode) {}

    // Displays the game instructions to the player.
    void showInstructions();

    // Runs the main game loop.
    void run() override;

    // Displays the game menu and handles user input to start or quit the game.
    bool menu();

    // Allows the player to choose the game screen/level.
    int chooseGameScreen();

    // Toggles the color mode on or off.
    void toggleColorMode();

    // Pauses the game and waits for the player to resume.
    void pauseGame();

    // Starts the game for the selected level.
    void startGame(int screen_index) override;

    // Handles game-over logic when the player loses.
    void loose() override;

    // Updates Mario's actions based on key presses from the user.
    void updateAction(int key) override;


};