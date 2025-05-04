
#include "HumanInputGame.h"
#include "mario.h"


void Mario::drawMario() {
	if (isSilentMode)
		return;
	if (HumanInputGame::getColorMode()) {
		// Set Mario's color to red when colorMode is enabled
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameCon::COLOR_MARIO);
	}
	else {
		// Default color when colorMode is disabled
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	myMario.draw(MARIO);

	// Reset to the default color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Mario::setStartingMario()
{
	isWon = false;
	fallFromTooHigh = false;
	usingHammer = false;
	isHasHammer = false;
	// Retrieve starting position from the board
	MARIO_START_X = myMario.getBoard()->getMarioStartPosition().x;
	MARIO_START_Y = myMario.getBoard()->getMarioStartPosition().y;

	// Erase Mario's character from the starting position
	myMario.getBoard()->eraseChar(MARIO_START_X, MARIO_START_Y);
	myMario.setXY(MARIO_START_X, MARIO_START_Y);

	dir = { 0,0 };								// Set stating direction for Mario
	prevChar = ' ';									// Reset the previous char prameter
}

char Mario::keyPressed(char key) {
	for (size_t i = 0; i < numKeys; i++)
	{
		if (std::tolower(key) == keys[i]) {
			changeDir(i);
			return key;
		}
	}
	if (key == 'p'&&isHasHammer) {
		usingHammer = true;
		return key;
	}
	return '\0';
}

void Mario::changeDir(int dirIndex)
{
	if(dir != GameCon::directions[dirIndex]) 
		prevDir = dir;
	dir = GameCon::directions[dirIndex];
	return;
}

bool Mario::isAboveFloor() const{
	return  (ch_below == '=' || ch_below == '<' || ch_below == '>');
}

bool Mario::isUnderneathFloor()
{
	return  (ch_above == '=' || ch_above == '<' || ch_above == '>');
}

// Check if Mario is on a ladder
bool Mario::isOnLadder() const
{
	return (ch_curr == 'H' || two_ch_below == 'H');	// Returns true only if mario is on the letter 'H' or above it
}

void Mario::move()
{
	int x = myMario.getX();
	int y = myMario.getY();
	static bool isClimbing = false;
	static bool isJumping = false;
	ch_curr = myMario.getBoard()->getChar(x, y);
	ch_above = myMario.getBoard()->getChar(x, y - 1);
	ch_below = myMario.getBoard()->getChar(x, y + 1);
	ch_left = myMario.getBoard()->getChar(x - 1, y);
	ch_right = myMario.getBoard()->getChar(x + 1, y);
	two_ch_below = myMario.getBoard()->getChar(x, y + 2);

	handleIlligalMove();
	checkState();
	updateByState();

	updateNextMove();
	updatePreviousChar();
}

void Mario::jump() {

	if (prevDir.x == -1) {
		dir = jumpDirections[leftJump[jumpStep]];
		jumpStep++;
	}
	else if (prevDir.x == 1) {
		dir = jumpDirections[rightJump[jumpStep]];
		jumpStep++;
	}
	else
	{
		dir = GameCon::directions[regularJump[jumpStep]];
		i++;
		if (i == 2 || i == 4) {
			jumpStep++;
			if (i == 4)
				i = 0;
		}
	}

	if (jumpStep == 2) {
		jumpStep = 0;
	}
}

void Mario::checkState() {
	if (isClimbingUp())
		state = MarioState::ClimbingUP;
	else if (isClimbingDown())
		state = MarioState::ClimbingDown;
	else if (isJumping())
		state = MarioState::Jumping;
	else if (isFalling())
		state = MarioState::Falling;
	else
		state = MarioState::Walking_or_Staying;
}

void Mario::updateByState()
{
	switch (state)
	{
	case MarioState::ClimbingUP:
		climbUp();
		/*	std::cout << "climbUPPPP";*/
		break;
	case MarioState::ClimbingDown:
		climbDown();
		/*	std::cout << "climbDOWNNNN";*/
		break;
	case MarioState::Jumping:
		jump();
		break;
	case MarioState::Falling:
		fall();
		break;
	case MarioState::Walking_or_Staying:
		walkOrStay();
		break;
	}
}



bool Mario::isClimbingUp() const
{
	return ((ch_curr == 'H' && dir.x == 0 && dir.y == -1) ||
		(ch_curr == 'H' && ch_below == 'H' && dir.y != 1)) &&
		!(state == MarioState::Jumping); // Prevent climbing while jumping
}

void Mario::climbUp()
{
	dir = { 0,-1 };
	if (isUnderneathFloor()) {
		myMario.setXY(myMario.getX(), myMario.getY() + dir.y - 1);
		dir = { 0,0 };
	}
}


bool Mario::isClimbingDown()const
{
	return ((dir.x == 0 && dir.y == 1 && isOnLadder()) ||
		(ch_below == 'H' && dir.x == 0 && dir.y == 1)) &&
		!(state == MarioState::Jumping || state == MarioState::Falling); // Prevent climbing while jumping or falling}
}

void Mario::climbDown()
{
	dir = { 0,1 };
	if (ch_below == '=' && two_ch_below == 'H') {
		myMario.setXY(myMario.getX(), myMario.getY() + dir.y + 1);
	}

}

// Handle the Mario's walking and standing
void Mario::walkOrStay()
{
	if (fallCounter >= HIGH_FALL) {								// If fall from high places, mario lose life
		fallFromTooHigh = true;
	}

	fallCounter = 0;														// Reset count_jump counter
	dir.y = 0;
}

bool Mario::isJumping()const
{
	if (jumpStep == 0)
	{
		return ((isAboveFloor() && dir.y == -1 && ch_curr != 'H') || i > 0);
	}
	return (jumpStep > 0);
}

bool Mario::isFalling() const
{
	return (ch_below == ' ');
}
void Mario::fall()
{
	dir = { 0,1 };
	fallCounter++;

}
void Mario::handleIlligalMove()
{
	if (isUnderneathFloor() && !isOnLadder() && dir.y == -1) {					// Under roof - mario can't jump
		dir.y = 0;
	}

	if (ch_left == 'Q' && dir.x == -1) {										// Linked to a wall - mario can't pass
		dir.x = 0;
	}

	if (ch_right == 'Q' && dir.x == 1) {
		dir.x = 0;			// Linked to a wall - mario can't pass
	}

	if (isAboveFloor() && two_ch_below != 'H' && dir.y == 1) {				// Above floor - mario can't go down
		dir.y = 0;
	}
	
}
// Updating the movement of the barrel for the next loop according to the position and the direction
void Mario::updateNextMove()
{
	int newX = myMario.getX() + dir.x;
	int newY = myMario.getY() + dir.y;

	if (myMario.getBoard()->getChar(newX, newY) == 'Q' || myMario.getBoard()->getChar(newX, newY) == '=') { 					// Update the next move by the board size	
		newX = myMario.getX();
		newY = myMario.getY();
	}
	if (myMario.getBoard()->getChar(newX, newY) == '$' || ch_below == '$')		// Check if mario reached pauline	
	{
		isWon = true;
		return;
	}

	myMario.setXY(newX, newY);
	ch_curr = myMario.getBoard()->getChar(newX, newY);
}

void Mario::printLives()
{
	gotoxy(5, 3);
	std::cout << lives;
	std::cout.flush(); // Ensure output is flushed immediately 
}

