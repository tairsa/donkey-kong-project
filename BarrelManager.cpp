#include "BarrelManager.h"

const std::vector<Barrel>& BarrelManager::getBarrels() const {
    return barrels;
}

const std::vector<Barrel>& BarrelManager::getDeadBarrels() const {
    return deadBarrels;
}

void BarrelManager::spawnBarrel() {

    int startX = (rand() % 2 == 0) ? 38 : 44; // Randomly choose a start position
    int startY = 4;
    barrels.push_back(Barrel(board, startX, startY,isSilentMode)); // Create and add barrel to the list
}

void BarrelManager::drawBarrels() {
    for (auto& barrel : barrels) {
        barrel.drawBarrel(); // Draw each barrel
    }
}

void BarrelManager::manageBarrelSpawning() {
    spawnCounter++;
    if (spawnCounter % 20 == 0) { // Spawn a barrel every 20 game ticks
        spawnBarrel();
    }
}

void BarrelManager::updateBarrels() {
    for (auto it = barrels.begin(); it != barrels.end();) {
        it->eraseEnemy(); // Erase the barrel at its previous position
        it->move();        // Update barrel position and state

        // Check if the barrel is in the "exploding" state
        if (it->getState() == Barrel::exploding) {
            deadBarrels.push_back(*it); // Move to deadBarrels vector
            it = barrels.erase(it);    // Remove from active barrels
        }
        else {
            ++it;
        }
    }
}

void BarrelManager::delDeadBarrels() {
    for (auto it = barrels.begin(); it != barrels.end();) {
        // Check if the barrel is in the "explode" state
        if (it->getAlive() == false) {
            it = barrels.erase(it); // Remove barrel and get the next iterator
        }
        else {
            it->drawBarrel();       // Draw the barrel at its new position
            ++it;                   // Move to the next barrel
        }
    }
}

void BarrelManager::eraseBarrels() {
    for (auto& barrel : barrels) {
        barrel.eraseEnemy(); // Erase barrel from screen
    }
}

void BarrelManager::clearBarrels() {
    barrels.clear(); // Assuming barrels are stored in a vector
}

void BarrelManager::clearDeadBarrels() {
    deadBarrels.clear();
}

void BarrelManager::eraseHitBarrels(int targetX, int targetY) {
    // Modify the vector to remove the barrel at the given coordinates
    for (auto it = barrels.begin(); it != barrels.end(); ++it) {
        if (it->getX() == targetX && it->getY() == targetY) {
            it->eraseEnemy();
            barrels.erase(it);  // Erase the barrel at the iterator position
            break;  // Exit the loop after removing the barrel
        }
    }
}

bool BarrelManager::isBarrelAt(int x, int y) const
{
    for (const auto& barrel : barrels)
    {
        if (barrel.getX() == x && barrel.getY() == y)
        {
            return true; // A barrel is at this position
        }
    }
    return false;
}