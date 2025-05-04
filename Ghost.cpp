#include "Ghost.h"
#include "HumanInputGame.h"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time() to seed random number generator



void Ghost::updateDirection()
{
    ch_left = enemy.getBoard()->getChar(enemy.getX() - 1, enemy.getY());
    ch_right = enemy.getBoard()->getChar(enemy.getX() + 1, enemy.getY());
    ch_left_below = enemy.getBoard()->getChar(enemy.getX() - 1, enemy.getY() + 1);
    ch_right_below = enemy.getBoard()->getChar(enemy.getX() + 1, enemy.getY() + 1);


    if (rand() % 100 < 5 && isDuringCollision == false)
    {
        reverseDirection();
    }

}

void Ghost::reverseDirection()
{
    dir.x = -dir.x;
}

void Ghost::move()
{
    int x = enemy.getX();
    int y = enemy.getY();
    ch_curr = enemy.getBoard()->getChar(x, y);
    ch_above = enemy.getBoard()->getChar(x, y - 1);
    ch_below = enemy.getBoard()->getChar(x, y + 1);
    ch_left = enemy.getBoard()->getChar(x - 1, y);
    ch_right = enemy.getBoard()->getChar(x + 1, y);
    two_ch_below = enemy.getBoard()->getChar(x, y + 2);

    if (isOnLadder() == true)
        climbLadder();
    if (isOnLadder() == false)
        updateDirection();

    // Calculate the new position
    int newX = enemy.getX() + dir.x;
    int newY = enemy.getY() + dir.y;


    char below = enemy.getBoard()->getChar(newX, newY + 1);

    if (below == '=' || below == '>' || below == '<' || OnLadder)
    {
        // Move to the new position
        if (!isSilentMode)
             enemy.erase(); // Erase current position
        enemy.setXY(newX, newY);
        if(!isSilentMode)
            enemy.draw(GHOST); // Draw at the new position
    }
    else
    {
        // Reverse direction if hitting the boundary
        reverseDirection();
    }

}
void Ghost::drawGhost()
{
    if (isSilentMode) {
        return;
    }
    if (HumanInputGame::getColorMode()) {
        // Set Barrel color when colorMode is enabled
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameCon::COLOR_GHOST);
    }
    else {
        // Default color when colorMode is disabled
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    enemy.draw(GHOST);  // Draw the ghost character

    // Reset to the default color (white)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Function to check for collisions with another Ghost
bool Ghost::checkCollisionWithOtherGhost(const Ghost* otherGhost) const {
    if (enemy.getX() == otherGhost->getX() && enemy.getY() == otherGhost->getY()) {
        return true;
    }
    return false;
}
