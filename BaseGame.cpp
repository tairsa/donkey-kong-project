#include "BaseGame.h"
#include "Windows.h"





void BaseGame::restartLevel()
{
	
	board.clearLegend();
	shouldUpdateLegend = true;
	board.reset();
	board.printScreen(board.getGameBoard());


	// Reset Mario's position and state
	mario.setStartingMario();
	flashMario();
	barrelManager.clearBarrels();
	ghostManager.clearGhosts();

	initializeGhosts();
}

bool BaseGame::isTouchingBarrel() const {
	int marioX = mario.getX();
	int marioY = mario.getY();

	// Iterate over barrels and check for collision
	for (const auto& barrel : barrelManager.getBarrels()) {
		if (barrel.getX() == marioX && barrel.getY() == marioY) {
			return true; // Mario is touching this barrel
		}
	}
	return false; // No collision detected
}

// Checks if Mario successfully completed the level
bool BaseGame::wonTheLevel()
{

	if (mario.getIfWon())
	{
		if (playing_mario) {
			mario.setStartingMario();
			board.resetHammerPosition();
			board.clearLegend();
			board.resetLegendPosition();
			barrelManager.clearBarrels();
			ghostManager.clearGhosts();
			board.resetGhostPositions();
			board.resetSpecialGhostPositions();
			flashMario();
		}
		else {
			mario.resetLives();
			board.printScreen(board.getWinningBoard());
			Sleep(2000);
			mario.setStartingMario();
			board.resetLegendPosition();
			resetScore();
			barrelManager.clearBarrels();
			ghostManager.clearGhosts();
			board.resetGhostPositions();
			board.resetSpecialGhostPositions();
			board.setColorMode(false);
			board.printScreen(board.getStartBoard());
		}
		return true;
	}
	else
		return false;
}

// Checks if Mario died from a barrel (hit or explosion)
void BaseGame::updateIfDiedByBarrel()
{
	int marioPosX;
	int marioPosY;
	marioPosX = mario.getX();
	marioPosY = mario.getY();

	// Check collisions with active barrels
	for (const auto& barrel : barrelManager.getBarrels())
	{
		int barrelPosX = barrel.getX();
		int barrelPosY = barrel.getY();

		if (marioPosX == barrelPosX && marioPosY == barrelPosY)
		{
			mario.died();
			currIterationLostLife = true;
			if (mario.getLives() == 0)
				playing_mario = false;
			else
				restartLevel();
			return;
		}
	}

	// Check explosion radius for dead barrels
	for (const auto& barrel : barrelManager.getDeadBarrels())
	{
		int explosionX = barrel.getExplosionX();
		int explosionY = barrel.getExplosionY();

		if (abs(explosionX - marioPosX) <= EXPLOSION_RADIUS &&
			abs(explosionY - marioPosY) <= EXPLOSION_RADIUS)
		{
			mario.died();
			currIterationLostLife = true;
			if (mario.getLives() == 0)
				playing_mario = false;
			else
				restartLevel();
			return;
		}
	}
}

void BaseGame::updateIfDiedByGhost()
{
	int marioPosX;
	int marioPosY;
	marioPosX = mario.getX();
	marioPosY = mario.getY();

	// Check collisions with active barrels
	for (const auto& ghost : ghostManager.getGhosts())
	{
		int ghostPosX = ghost->getX();
		int ghostPosY = ghost->getY();

		if (marioPosX == ghostPosX && marioPosY == ghostPosY)
		{
			mario.died();
			currIterationLostLife = true;
			if (mario.getLives() == 0)
				playing_mario = false;
			else
				restartLevel();
			return;
		}
	}

}


void BaseGame::diedFromExplodedBarrel(int barrelPosX, int barrelPosY, int marioPosX, int marioPosY, int i)
{
	// Check if the index 'i' is within the valid range of barrels vector
	if (i >= 0 && i < barrelManager.getBarrels().size()) {
		bool is_exploded = !barrelManager.getBarrels()[i].getAlive();  // Check if the barrel has exploded
		if (is_exploded) {
			if (abs(barrelPosX - marioPosX) <= EXPLOSION_RADIUS && abs(barrelPosY - marioPosY) <= EXPLOSION_RADIUS) {
				mario.died();
				currIterationLostLife = true;
				restartLevel();
			}
		}
	}
}

void BaseGame::checkIfMarioFellFromHigh()
{
	if (mario.getFallFromTooHigh() == true) {
		mario.died();
		currIterationLostLife = true;
		restartLevel();
	}
}

void BaseGame::flashMario() {
	int flashCount = 5; // Number of flashes
	int flashDelay = 200; // Delay between flashes in milliseconds

	for (int i = 0; i < flashCount; ++i) {
		mario.drawMario();  // Draw Mario
		Sleep(flashDelay);  // Wait
		mario.eraseMario(); // Erase Mario
		Sleep(flashDelay);  // Wait
	}

	mario.drawMario(); // Ensure Mario is visible after the flashing
}


void BaseGame::isCollectingHammer()
{
	int posX = mario.getX();
	int posY = mario.getY();
	int posHammerX = board.getStartHammerPositionX();
	int posHammerY = board.getStartHammerPositionY();
	if (posX == posHammerX && posY == posHammerY)
	{
		mario.setIsHasHammer(true);
		board.setChar(posHammerX, posHammerY, ' ');
		shouldUpdateLegend = true;
	}
}

void BaseGame::checkAndUseHammer()
{
	if (mario.isUsingHammer())
	{
		mario.setUsingHammer(false);

		// Use previous direction if Mario is stationary //ask TAIR
		const GameCon::Direction& dirHammer = (mario.getDirection().x == 0 && mario.getDirection().y == 0)
			? mario.getPrevDirection()
			: mario.getDirection();

		if (dirHammer.x == 0 && dirHammer.y == 0) return;
		useHammer(mario.getX(), mario.getY(), dirHammer);
	}
}


void BaseGame::useHammer(int marioX, int marioY, const GameCon::Direction& dir)
{
	// Check two tiles in the direction Mario is facing
	for (int i = 1; i <= 2; ++i)
	{
		int targetX = marioX + dir.x * i;
		int targetY = marioY + dir.y * i;

		if (barrelManager.isBarrelAt(targetX, targetY))
		{
			handleHammerHitBarrel(targetX, targetY);
			addScore();
			return;  // Stop checking after hitting the barrel
		}

		if (ghostManager.isGhostAt(targetX, targetY))
		{
			handleHammerHitGhost(targetX, targetY);
			addScore();
			return;  // Stop checking after hitting the ghost
		}
	}
}

void BaseGame::handleHammerHitBarrel(int targetX, int targetY)
{
	barrelManager.eraseHitBarrels(targetX, targetY);
}

void BaseGame::handleHammerHitGhost(int targetX, int targetY)
{
	ghostManager.eraseHitGhosts(targetX, targetY);
}

void BaseGame::updateLegendState() {
	int marioScore = score;
	int marioLives = mario.getLives();
	bool hasHammer = mario.getHasHammer();

	if (shouldUpdateLegend)
	{
		board.updateLegend(marioScore, marioLives, hasHammer);
		score = marioScore;
		shouldUpdateLegend = false;
	}
}

void BaseGame::checkIfOffLimits() {
	if (mario.isCharBelowLimit()) {
		mario.died();
		currIterationLostLife = true;
		restartLevel();
	}

}



void BaseGame::initializeGhosts()
{
	for (const auto& pos : board.getStartGhostPositions()) { // Assuming `getStartGhostPositions()` returns `start_pos_ghosts_vec`
		ghostManager.addGhost(pos.x, pos.y, false,isSilentMode);

	}
	for (const auto& pos : board.getStartSpecialGhostPositions()) { // Assuming `getStartGhostPositions()` returns `start_pos_ghosts_vec`
		ghostManager.addGhost(pos.x, pos.y, true,isSilentMode);

	}

}