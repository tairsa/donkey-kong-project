#pragma once
#include <cstring> //for memcpy
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include "utils.h"
#include "GameCon.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>



class Board
{
	bool colorMode = false;
	static constexpr const char* RESET_COLOR = "\033[0m";
	static constexpr const char* RED = "\033[31m";
	static constexpr const char* CYAN = "\033[36m";
	static constexpr const char* PINK = "\033[95m";
	static constexpr const char* DARK_GRAY = "\033[90m";

	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;

	std::string directory = ".";

	struct Position {
		int x = 0;
		int y = 0;
	};
	bool isSilentMode = false;

	char originalBoard[MAX_Y][MAX_X + 1];
	//const char* originalBoard[MAX_Y] = {
	//	// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
	//	  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
	//	  "Q  *   *                                                                       Q", // 1
	//	  "Q * * * *                                $                                     Q", // 2
	//	  "Q  *   *                 ================================                      Q", // 3
	//	  "Q   * *                     H            &            H                        Q", // 4
	//	  "Q    *                      H     ====<=====>====     H                        Q", // 5
	//	  "Q                           H                         H                        Q", // 6
	//	  "Q                           H                         H                        Q", // 7
	//	  "Q                           H                         H                        Q", // 8
	//	  "Q      <<<=======================<<           >>>====================>>>       Q", // 9
	//	  "Q                 H                                                   H        Q", // 10
	//	  "Q                 H                                                   H        Q", // 11
	//	  "Q                 H                                                   H        Q", // 12
	//	  "Q                 H                                                   H        Q", // 13
	//	  "Q     >>==========================                     <<===============<<     Q", // 14
	//	  "Q          H                                                       H           Q", // 15
	//	  "Q          H                                                       H           Q", // 16
	//	  "Q          H                                                       H           Q", // 17
	//	  "Q          H                                                       H           Q", // 18
	//	  "Q      =========>>===========>>===========               <<==================  Q", // 19
	//	  "Q                 H                H                        H                  Q", // 20
	//	  "Q                 H                H                        H                  Q", // 21
	//	  "Q                 H                H                        H                  Q", // 22
	//	  "Q==============================================================================Q", // 23
	//	  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	//};

	char start_screen[MAX_Y][MAX_X + 1] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"********************************************************************************", //0
			"*                                                                              *", //1
			"*                                                                              *", //2
			"*                                                                              *", //3
			"*        ____   ___  _   _ _  _________   __  _  _____  _   _  ____            *", //4
			"*       |  _ \\ / _ \\| \\ | | |/ | ____\\ \\ / / | |/ / _ \\| \\ | |/ ___|           *", //5
			"*       | | | | | | |  \\| | ' /|  _|  \\ V /  | ' | | | |  \\| | |  _            *", //6
			"*       | |_| | |_| | |\\  | . \\| |___  | |   | . | |_| | |\\  | |_| |           *", //7
			"*       |____/ \\___/|_| \\_|_|\\_|_____| |_|   |_|\\_\\___/|_| \\_|\\____|           *", //8
			"*                                                                              *", //9
			"*                                                                              *", //10
			"*                                                                              *", //11
			"*          (1) Start new game                                                  *", //12
			"*          (8) instructions and keys                                           *", //13
			"*          (9) Exit                                                            *", //14
			"*          (c) Color Mode:                                                     *", //15
			"*                                                                              *", //16
			"*                                                                              *", //17
			"*                                                                              *", //18
			"*                                                                              *", //19
			"*                                                                              *", //20
			"*                                                                              *", //21
			"*                                                                              *", //22
			"*                                                                              *", //23
			"********************************************************************************"  //24
	};



	char winning_screen[MAX_Y][MAX_X + 1] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"********************************************************************************",//0
		"*                                                                              *",//1  
		"*                                                                              *",//2
		"*                __        _____ _   _ _   _ _____ ____                        *",//3
		"*                \\ \\      / |_ _| \\ | | \\ | | ____|  _ \\                       *",//4
		"*                 \\ \\ /\\ / / | ||  \\| |  \\| |  _| | |_) |                      *",//5
		"*                  \\ V  V /  | || |\\  | |\\  | |___|  _ <                       *",//6
		"*                   \\_/\\_/  |___|_| \\_|_| \\_|_____|_| \\_\\                      *",//7 
		"*                                                                              *",//8 
		"*                                                                              *",//9 
		"*                                                                              *",//10
		"*                                                                              *",//11
		"*                                                                              *",//12
		"*                            YOU SAVED PAULINE!                                *",//13
		"*                                                                              *",//14
		"*                                                                              *",//15
		"*                                                                              *",//16
		"*                                                                              *",//17
		"*                                                                              *",//18
		"*                                                                              *",//19
		"*                                                                              *",//20
		"*                                                                              *",//21
		"*                                                                              *",//22
		"*                                                                              *",//23
		"********************************************************************************" //24


	};

	char instruction_screen[MAX_Y][MAX_X + 1] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "********************************************************************************",//0
		 "*                                                                              *",//1
		 "*                          GAME INSTRUCTIONS AND KEYS                          *",//2
		 "*                                                                              *",//3
		 "* Instructions:                                                                *",//4
		 "* - The goal is to help Mario (@) rescue Pauline ($) while avoiding barrels.   *",//5
		 "* - Donkey Kong (&) throws barrels that move across floors (<, >, =).          *",//6
		 "* - Use ladders (H) and jumps to avoid barrels and reach Pauline.              *",//7 
		 "* - Mario loses a life if hit by a barrel or falls from a height of 5 lines.   *",//8 
		 "* - Mario has 3 lives; losing all ends the game.                               *",//9 
		 "* - Barrels explode if they fall 8 lines, damaging nearby areas.               *",//10 
		 "* - The stage restarts after losing a life.                                    *",//11 
		 "* - Reach Pauline to win the stage.                                            *",//12
		 "*                                                                              *",//13 
		 "*                                                                              *",//14 
		 "* Keys:                                                                        *",//15 
		 "* - A/a: Move left                     - D/d: Move right                       *",//16 
		 "* - W/w: Jump or climb up a ladder     - X/x: Climb down a ladder              *",//17 
		 "* - S/s: Stay in place                 - ESC: Pause, press again to resume     *",//18 
		 "* - 9: Exit the game                                                           *",//19 
		 "*                                                                              *",//20 
		 "*                                                                              *",//21 
		 "*                                            < Press 'R' to return to menu >   *",//22 
		 "*                                                                              *",//23 
		 "********************************************************************************" //24
	};
	char goodBye_screen[MAX_Y][MAX_X + 1] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "********************************************************************************",//0
		 "*                                                                              *",//1
		 "*                                                                              *",//2
		 "*                                                                              *",//3
		 "*                                                                              *",//4
		 "*                                                                              *",//5
		 "*                                                                              *",//6
		 "*                                                                              *",//7
		 "*                                                                              *",//8
		 "*             **************************************************               *",//9
		 "*             *  ____                 _   ____               _ *               *",//10
		 "*             * / ___| ___   ___   __| | | __ ) _   _  ___  | |*               *",//11
		 "*             *| |  _ / _ \\ / _ \\ / _` | |  _ \\| | | |/ _ \\ | |*               *",//12
		 "*             *| |_| | (_) | (_) | (_| | | |_) | |_| |  __/ |_|*               *",//13
		 "*             * \\____|\\___/ \\___/ \\__,_| |____/ \\__, |\\___| (_)*               *",//14
		 "*             *                                 |___/          *               *",//15
		 "*             **************************************************               *",//16         
		 "*                                                                              *",//17
		 "*                                                                              *",//18 
		 "*                                                                              *",//19 
		 "*                                                                              *",//20 
		 "*                                                                              *",//21 
		 "*                                                                              *",//22 
		 "*                                                                              *",//23 
		 "********************************************************************************" //24
	};

	char loosing_screen[MAX_Y][MAX_X + 1] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "********************************************************************************",//0
		 "*                                                                              *",//1
		 "*                                                                              *",//2
		 "*         ____   ____  ___ ___    ___       ___   __ __    ___  ____           *",//3
		 "*        /    | /    ||   |   |  /  _]     /   \\ |  |  |  /  _]|    \\          *",//4
		 "*       |   __||  o  || _   _ | /  [_     |     ||  |  | /  [_ |  D  )         *",//5
		 "*       |  |  ||     ||  \\_/  ||    _]    |  O  ||  |  ||    _]|    /          *",//6
		 "*       |  |_ ||  _  ||   |   ||   [_     |     ||  :  ||   [_ |    \\          *",//7 
		 "*       |     ||  |  ||   |   ||     |    |     | \\   / |     ||  .  \\         *",//8 
		 "*       |___,_||__|__||___|___||_____|     \\___/   \\_/  |_____||__|\\_|         *",//9 
		 "*                                                                              *",//10
		 "*                                                                              *",//11
		 "*                                                                              *",//12
		 "*                                                                              *",//13
		 "*                            TRY AGAIN NEXT TIME                               *",//14
		 "*                                                                              *",//15
		 "*                                                                              *",//16
		 "*                                                                              *",//17
		 "*                                                                              *",//18
		 "*                                                                              *",//19
		 "*                                                                              *",//20
		 "*                                                                              *",//21
		 "*                                                                              *",//22
		 "*                                                                              *",//23
		 "********************************************************************************" //24

	};

	char chooseBoard_screen[MAX_Y][MAX_X + 1] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "********************************************************************************",//0
		 "*                                                                              *",//1
		 "*    ____                               ___        _   _                       *",//2
		 "*   / ___|  ___ _ __ ___  ___ _ __     / _ \\ _ __ | |_(_) ___  _ __  ___   _   *",//3
		 "*   \\___ \\ / __| '__/ _ \\/ _ \\ '_ \\   | | | | '_ \\| __| |/ _ \\| '_ \\/ __| (_)  *",//4
		 "*    ___) | (__| | |  __/  __/ | | |  | |_| | |_) | |_| | (_) | | | \\__ \\  _   *",//5
		 "*   |____/ \\___|_|  \\___|\\___|_| |_|   \\___/| .__/ \\__|_|\\___/|_| |_|___/ (_)  *",//6
		 "*                                           |_|                                *",//7
		 "*                                                                              *",//8
		 "*                                                                              *",//9
		 "*                                                                              *",//10
		 "*                                                                              *",//11
		 "*                                                                              *",//12
		 "*                                                                              *",//13
		 "*                                                                              *",//14
		 "*                                                                              *",//15
		 "*                                                                              *",//16         
		 "*                                                                              *",//17
		 "*                                                                              *",//18 
		 "*                                                                              *",//19 
		 "*                                                                              *",//20 
		 "*                                                                              *",//21 
		 "*                                                                              *",//22 
		 "*                                                                              *",//23 
		 "********************************************************************************" //24
	};



	char currentBoard[MAX_Y][MAX_X + 1]; // +1 for null terminator

	Position start_pos_mario{ 0,0 };
	Position start_pos_donkey{ 0,0 };
	Position start_pos_pauline{ 0,0 };
	Position start_pos_hammer{ 0,0 };
	Position start_pos_ghost{ 0,0 };
	std::vector<Position> start_pos_ghosts_vec;
	std::vector<Position> start_pos_special_ghosts_vec;

	struct Legend {
		int score = 0;
		int life = 3;
		std::string hammer = "OFF";
		Position pos_L{ 0,0 };
		int legendWidth;
		int legendHeight;

		Position pos_score_in_legend{ 0,0 };
		Position pos_life_in_legend{ 0,0 };
		Position pos_hammer_in_legend{ 0,0 };


		std::string str_score = "Score: "; 
		std::string str_life = "Life: "; 
		std::string str_hammer = "Hammer: "; 
	};

	Legend legend;

public:
	void setSilentMode(bool silent) { isSilentMode = silent; }
	void setColorMode(bool mode) { colorMode = mode; }
	void reset();
	static int getMaxX() { return MAX_X; }
	static int getMaxY() { return MAX_Y; }
	void getAllBoardFileNames(std::vector<std::string>& vec_to_fill) const;
	// This function resets the board to its original state
	void printScreen(const char screen[][MAX_X + 1]) const;
	bool load(const std::string& filename);
	void handleErrors(const std::ifstream& _file);
	void printScreenOptions(std::vector<std::string>& vec_to_fill) const;
	char getChar(int x, int y) const {
		return currentBoard[y][x];
	}
	void setChar(int x, int y, char ch) {
		currentBoard[y][x] = ch;
	}

	// Clears the character at the given position by restoring the original board character
	void eraseChar(int x, int y) {
		if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
			currentBoard[y][x] = ' ';
		}
	}
	const char(&getStartBoard() const)[MAX_Y][MAX_X + 1]{ return start_screen; }
	const char(&getGameBoard() const)[MAX_Y][MAX_X + 1]{ return currentBoard; }
	const char(&getWinningBoard() const)[MAX_Y][MAX_X + 1]{ return winning_screen; }
	const char(&getInstructionBoard() const)[MAX_Y][MAX_X + 1]{ return instruction_screen; }
	const char(&getGoodByeBoard() const)[MAX_Y][MAX_X + 1]{ return goodBye_screen; }
	const char(&getLoosingBoard() const)[MAX_Y][MAX_X + 1]{ return loosing_screen; }

	const std::vector<Position>& getStartGhostPositions() const { return start_pos_ghosts_vec; }
	const std::vector<Position>& getStartSpecialGhostPositions() const { return start_pos_special_ghosts_vec; }	
	void resetGhostPositions() {start_pos_ghosts_vec.clear();}
	void resetSpecialGhostPositions() {start_pos_special_ghosts_vec.clear();}

	const int getStartHammerPositionX() const { return start_pos_hammer.x; }
	const int getStartHammerPositionY() const { return start_pos_hammer.y; }
	Position getMarioStartPosition() const { return start_pos_mario; } 

	void setPositionsInLegend();

	void printLegend() const; 
	void updateLegend(int score, int lives, bool hasHammer); 
	void clearLegend() const;
	bool validateBoard(int marioCount, int paulineCount, int donkeyCount, int legendCount);

	void printCenteredMessage(const std::string& message) const;

	void resetLegendPosition() { legend.pos_L = { 0, 0 }; }
	void resetHammerPosition() { start_pos_hammer = { 0, 0 }; }
	int getLegendScore() { return legend.score; }

};




