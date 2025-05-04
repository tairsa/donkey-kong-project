#include "GhostManager.h"



void GhostManager::addGhost(int startX, int startY, bool isSpecial,bool silent)
{
    if (isSpecial) {
        ghosts.push_back(new SpecialGhost(board, startX, startY,silent));  // Create SpecialGhost
    }
    else {
        ghosts.push_back(new Ghost(board, startX, startY,silent));  // Create Normal Ghost
    }
}

void GhostManager::handleCollision(std::vector<Ghost*>::iterator it) {
    for (auto other = it + 1; other != ghosts.end(); ++other) { // Start from the next ghost
        if ((*it)->checkCollisionWithOtherGhost(*other)) {  // Fix: Dereferencing Ghost pointers properly
            (*it)->setIsDuringCollision(true);
            (*other)->setIsDuringCollision(true);
            (*it)->reverseDirection();
            (*it)->move();
            (*other)->reverseDirection();
            (*other)->move();
            (*it)->setIsDuringCollision(false);
            (*other)->setIsDuringCollision(false); 
        }
    }
}





void GhostManager::updateGhosts() {
    for (Ghost* ghost : ghosts) {
        ghost->move();  
    }

    for (auto it = ghosts.begin(); it != ghosts.end(); ++it) {
        Ghost* ghost = *it;   
        handleCollision(it); 
        /*ghost->drawGhost();*/    
    }
}


void GhostManager::eraseGhosts() {
    for (Ghost* ghost : ghosts) {
        ghost->eraseEnemy();
    }
}



void GhostManager::clearGhosts() {
    for (Ghost* ghost : ghosts) {
        delete ghost;  // Free memory
    }
    ghosts.clear();
}

void GhostManager::eraseHitGhosts(int targetX, int targetY) {
    for (auto it = ghosts.begin(); it != ghosts.end();) {
        if ((*it)->getX() == targetX && (*it)->getY() == targetY) {
            (*it)->eraseEnemy();
            delete* it;  // Free memory
            it = ghosts.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool GhostManager::isGhostAt(int x, int y) const
{
    for (Ghost* ghost : ghosts)
    {
        if (ghost->getX() == x && ghost->getY() == y)
        {
            return true; // A ghost is at this position
        }
    }
    return false;
}
GhostManager::~GhostManager() {
    clearGhosts();  // Ensure memory is freed
}