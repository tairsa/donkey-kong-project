#pragma once

#include "Point.h"
#include <Windows.h>
#include "utils.h"



/**
 * Represents Mario, the player-controlled character.
 * Handles movement, state transitions, and rendering.
 */
class Mario
{
	Point myMario;                           // Mario's position on the game board
	static constexpr char MARIO = '@';       // Character to represent Mario
	int MARIO_START_X = 1;  // Starting x-coordinate for Mario
	int MARIO_START_Y = 22; // Starting y-coordinate for Mario
	static constexpr int HIGH_FALL = 5;      // Maximum safe fall distance before taking damage
	static constexpr char keys[] = { 'w','a','x','d','s' }; // Movement keys for Mario
	static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);

	// Constants for handling jump directions
	enum eJump { UPRIGHT, DOWNRIGHT, UPLEFT, DOWNLEFT };
	static constexpr GameCon::Direction jumpDirections[] = { {1,-1},{1,1},{-1,-1},{-1,1} }; 
	static constexpr int rightJump[] = { UPRIGHT,DOWNRIGHT }; // Jump directions when facing right
	static constexpr int leftJump[] = { UPLEFT,DOWNLEFT };   // Jump directions when facing left
	static constexpr int regularJump[] = { (int)GameCon::eDir::UP,(int)GameCon::eDir::DOWN }; // Vertical jump directions

	GameCon::Direction dir{ 0, 0 };       // Current direction Mario is moving
	GameCon::Direction prevDir{ 0, 0 };  // Previous direction Mario moved
	int lives = 3;                       // Number of lives Mario has
	int fallCounter = 0;                 // Counter for tracking fall distance
	int jumpStep = 0;                    // Step index for jump sequences
	bool isWon = FALSE;                  // Flag indicating if Mario has won the level
	bool fallFromTooHigh = false;        // Flag indicating if Mario fell from a high distance
	int i = 0;                           // Auxiliary counter for jump timing

	// Enumeration for Mario's possible states
	enum class MarioState {
		ClimbingUP,         // Mario is climbing up a ladder
		ClimbingDown,       // Mario is climbing down a ladder
		Jumping,            // Mario is in the middle of a jump
		Falling,            // Mario is falling
		Walking_or_Staying  // Mario is walking or standing still
	};
	MarioState state = MarioState::Walking_or_Staying; // Current state of Mario

	// Characters surrounding Mario
	char ch_curr = ' ', ch_below = ' ', two_ch_below = ' ', ch_above = ' ', ch_left = ' ', ch_right = ' ';
	char prevChar = ' '; // Previous character at Mario's position

	bool isHasHammer = false;
	bool usingHammer = false;
	bool isSilentMode;

public:
	Mario(Board& board,bool silent) :myMario(MARIO_START_X, MARIO_START_Y),
	isSilentMode(silent) {
		myMario.setBoard(board);
	}

	void eraseMario() { if (!isSilentMode) { myMario.erase(); } } // Erases Mario's current position from the screen
	int getX() const { return myMario.getX(); } // Gets Mario's current x-coordinate
	int getY() const { return myMario.getY(); } // Gets Mario's current y-coordinate

	void drawMario();              // Draws Mario on the screen
	void setStartingMario();       // Resets Mario to the starting position and state
	char keyPressed(char key);     // Handles key press for Mario's movement
	void changeDir(int dirIndex);  // Changes Mario's direction based on input
	bool isAboveFloor() const;           // Checks if Mario is above a floor
	bool isUnderneathFloor();      // Checks if Mario is underneath a floor
	bool isOnLadder() const;       // Checks if Mario is on a ladder
	int getLives() { return lives; } // Returns the number of lives Mario has
	GameCon::Direction getDirection() const { return dir; }
	GameCon::Direction getPrevDirection() const {return prevDir;}
	void resetLives() { lives = 3; } // Resets Mario's lives to the default value
	void died() { --lives; }       // Decreases Mario's life count by one
	void move();                   // Updates Mario's position based on state and input
	void jump();                   // Handles Mario's jump logic
	void checkState();             // Checks and updates Mario's current state
	void updateByState();          // Updates Mario's behavior based on the current state

	bool isClimbingUp ()const;           // Checks if Mario is climbing up
	void climbUp();                // Handles Mario's climbing up logic
	bool isClimbingDown()const;         // Checks if Mario is climbing down
	void climbDown () ;              // Handles Mario's climbing down logic
	void walkOrStay();             // Handles Mario's walking or standing still logic
	bool isJumping()const;              // Checks if Mario is in the middle of a jump
	bool isFalling() const;              // Checks if Mario is falling
	void fall();                   // Handles Mario's falling logic
	void handleIlligalMove();      // Prevents illegal moves (e.g., walking into walls)
	void updateNextMove();         // Updates Mario's next position based on input
	void updatePreviousDir() { prevDir = dir; } // Updates Mario's previous direction
	void updatePreviousChar() { prevChar = ch_curr; } // Updates the previous character at Mario's position
	void printLives();             // Prints the number of lives Mario has left on the screen
	bool getIfWon() const { return isWon; } // Returns whether Mario has won the level
	bool getFallFromTooHigh() const { return fallFromTooHigh; } // Checks if Mario fell from too high
	bool getHasHammer() const { return isHasHammer; }
	bool isCharBelowLimit() const{ return ch_below == 'Q'; }
	void setIsHasHammer(bool b) { isHasHammer = b; }
	bool isUsingHammer() const { return usingHammer; }
	void setUsingHammer(bool value) { usingHammer = value; }
};
