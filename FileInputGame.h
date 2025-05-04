#pragma once

#include "BaseGame.h"

class FileInputGame:public BaseGame
{

    std::string currFileName = "";
    size_t nextDiedIteration = 0;
    bool unmatching_result_found = false;

public:
    // Constructor: Initializes the game with board, Mario, and managers.
    FileInputGame(bool silent) :BaseGame(silent) {
        isSilentMode = silent;
    }

    // Runs the main game loop.
    void run() override;

    // Starts the game for the selected level.
    void startGame(int screen_index) override;

    void loose() override;

    // Updates Mario's actions based on key presses from the user.
    void updateAction(int key) override;

    void reportResultError(const std::string& message, const std::string& filename, size_t iteration);

};