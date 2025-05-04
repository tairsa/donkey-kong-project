#include "SpecialGhost.h"

void SpecialGhost::climbLadder()
{
	int rnd = rand() % 2;
	checkState(rnd);
	updateByState();
}


bool SpecialGhost::isClimbingUp(int rand) const
{
	return (ch_curr == 'H' &&rand==0&&!isUnderneathFloor());
}

void SpecialGhost::climbUp()
{
	dir = { 0,-1 };
}

bool SpecialGhost::isClimbingDown(int rand) const
{
	return (rand == 1 && ((ch_curr == 'H' && !isAboveFloor()) || two_ch_below == 'H'));
}

void SpecialGhost::climbDown()
{
	dir = { 0,1 };
	if ((ch_below == '='|| ch_below == '>'|| ch_below == '<') && two_ch_below == 'H') {
		enemy.setXY(enemy.getX(), enemy.getY() + 2);
	}
}
void SpecialGhost::walk()
{
	if (isUnderneathFloor()) 
		enemy.setXY(enemy.getX(), enemy.getY() - 2);
	if (rand() % 2== 0) {
		dir = { 1,0 }; 
	}
	else
		dir = { -1,0 };  
}

void SpecialGhost::checkState(int rand)  {
	if (isClimbingUp(rand))
		state = GhostState::ClimbingUP;
	else if (isClimbingDown(rand))
		state = GhostState::ClimbingDown;
	else
		state = GhostState::Walking;
}

void SpecialGhost::updateByState()
{
	switch (state)
	{
	case GhostState::ClimbingUP:
		climbUp();
		OnLadder = true;
		break;
	case GhostState::ClimbingDown:
		climbDown();
		OnLadder = true;

		break;
	case GhostState::Walking:
		walk();
		OnLadder = false;
		break;
	}
}
