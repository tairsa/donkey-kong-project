#include "Barrel.h"  // Include the header file
#include "HumanInputGame.h" 




void Barrel::move()
{
    int x = enemy.getX();
    int y = enemy.getY();
    ch_curr = enemy.getBoard()->getChar(x, y);
    ch_above = enemy.getBoard()->getChar(x, y - 1);
    ch_below = enemy.getBoard()->getChar(x, y + 1);
    ch_left = enemy.getBoard()->getChar(x - 1, y);
    ch_right = enemy.getBoard()->getChar(x + 1, y);
    two_ch_below = enemy.getBoard()->getChar(x, y + 2);

    checkState();
    updateByState();
    updateNextMove();


}

bool Barrel::isFalling() const
{
    return(ch_below == ' ');
}

bool Barrel::isExplode() const
{
    return (fallDistance >= HIGH_FALL || ch_left == 'Q' || ch_right == 'Q'||ch_below=='Q');
}
bool Barrel::isRollingLeft() const {
    if (ch_below == '<')
        return true;
    if (ch_below == '=' && prevStateBeforeFalling == rollingL)
        return true;
    else
        return false;

}
bool Barrel::isRollingRight() const {
    if (ch_below == '>')
        return true;
    if (ch_below == '=' && prevStateBeforeFalling == rollingR)
        return true;
    else
        return false;

}



void Barrel::checkState()
{
    if (isFalling())
        currentState = falling;
    else if (isExplode())
        currentState = exploding;
    else if (isRollingLeft())
        currentState = rollingL;
    else
        currentState = rollingR;
}
void Barrel::rollRight() {
    dir = { 1,0 };
    fallDistance = 0;
    prevStateBeforeFalling = rollingR;
}

void Barrel::rollLeft()
{
    dir = { -1,0 };
    fallDistance = 0;
    prevStateBeforeFalling = rollingL;
}
void Barrel::fall()
{
    dir = { 0,1 };
    fallDistance++;

}
void Barrel::explode() {
    dir = { 0,0 };
    alive = false;
    explosionX = enemy.getX();
    explosionY = enemy.getY();

}




void Barrel::updateByState()
{
    switch (currentState)
    {
    case rollingR:
        rollRight();
        break;
    case rollingL:
        rollLeft();
        break;
    case falling:
        fall();
        break;
    case exploding:
        explode();
        break;

    }
}
// Updating the movement of the barrel for the next loop according to the position and the direction
void Barrel::updateNextMove()
{
    int newX = enemy.getX() + dir.x;
    int newY = enemy.getY() + dir.y;


    enemy.setXY(newX, newY);
}



// drawBarrel definition
void Barrel::drawBarrel() {
    if (isSilentMode)
        return;
    if (HumanInputGame::getColorMode()) {
        // Set Barrel color when colorMode is enabled
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameCon::COLOR_BARREL);
    }
    else {
        // Default color when colorMode is disabled
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    enemy.draw(BARREL);  // Draw the barrel character

    // Reset to the default color (white)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
