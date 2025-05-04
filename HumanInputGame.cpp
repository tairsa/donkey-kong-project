#include "HumanInputGame.h"
#include "Windows.h"

bool HumanInputGame::colorMode = false;
// Displays the game instructions screen to the player


void HumanInputGame::showInstructions()
{
	board.printScreen(board.getInstructionBoard());
	bool in_instruction_screen = true;								 // Flag to keep the instruction screen active until the user decides to exit

	while (in_instruction_screen)									 // Loop to wait for user input while on the instruction scree
	{
		if (_kbhit())
		{
			int key = std::tolower(_getch());
			if (key == (int)GameCon::eKeys::RETURN_BACK)
				in_instruction_screen = false;
		}
	}
	board.printScreen(board.getStartBoard());
}



// Manages the overall flow of the game
void HumanInputGame::run() 
{
	system("mode con cols=80 lines=25");			// Set the console size to be 80X25
	ShowConsoleCursor(false);						// Hides the console cursor to improve visual appearance during the game

	board.getAllBoardFileNames(files_names_vec);
	board.printScreen(board.getStartBoard());		// Displays the starting board on the screen

	bool playing = true;

	while (playing) {
		// Main game loop: keeps running while the game is active
		gotoxy(26, 15);
		if (colorMode) {
			std::cout << "ON ";

		}
		else {
			std::cout << "OFF";
		}
		playing = menu();
	}
}


//Displays the game menu and handles user input to start or quit the game
bool HumanInputGame::menu()
{
	if (_kbhit())									// Checks if a key has been pressed
	{
		int key = std::tolower(_getch());							// Reads the key that was pressed
		int screen_index;
		switch (key) {
		case((int)GameCon::eKeys::START_NEW_GAME):					// User pressed the key to start a new game
			board.printScreenOptions(files_names_vec);
			screen_index = chooseGameScreen();
			if (screen_index == -1)
				break;
			startGame(screen_index);
			colorMode = false;
			board.setColorMode(false);
			break;
		case((int)GameCon::eKeys::INSTRUCTIONS):				// User pressed the key to view instructions
			showInstructions();
			break;
		case((int)GameCon::eKeys::EXIT_GAME):       			// User pressed the key to exit the game
			board.printScreen(board.getGoodByeBoard());
			Sleep(1000);
			return false;							// Exit the menu loop and terminate the game
			break;
		case((int)GameCon::eKeys::TOGGLE_COLORS):					// User pressed the key to start a new game
			toggleColorMode();
			break;
		}

	}
	return true;
}

int HumanInputGame::chooseGameScreen()
{
	while (true)										// Checks if a key has been pressed
	{
		if (_kbhit())
		{
			int key = _getch() - '0';									// Reads the key that was pressed
			if (!files_names_vec.empty() && key > 0 && key <= files_names_vec.size())
			{
				key--;											// The array start from zero
				return key;
			}
			else if (key == (int)GameCon::eKeys::EXIT_GAME) {
				return -1;
			}

			break;
		}
	}
	return -1;													// Prevent warnings
}


void HumanInputGame::toggleColorMode()
{

	colorMode = !colorMode;// Toggle the color mode
	board.setColorMode(colorMode);




}


void HumanInputGame::pauseGame()
{
	bool pause_on = true;
	int key;

	const char* pauseMessage = "PAUSED - Press ESC to resume";
	int centerX = ((int)GameCon::GAME_WIDTH - strlen(pauseMessage)) / 2;
	int centerY = (int)GameCon::GAME_HEIGHT / 2;

	gotoxy(centerX, centerY);  // Position the pause message
	std::cout << pauseMessage;

	while (pause_on)
	{
		if (_kbhit())
		{
			key = std::tolower(_getch());
			if (key == (int)GameCon::eKeys::PAUSE)
				pause_on = false;  // Exit the pause loop
		}

		Sleep(50);
	}

	// After pausing, clear the screen and redraw the game board
	system("cls");  // Clear the screen

	// Redraw the game state after unpausing
	board.printScreen(board.getGameBoard());
	shouldUpdateLegend = true;
}

void HumanInputGame::startGame(int screen_index)  {
	bool valid_file;
	playing_mario = true;							    // Indicates that the Mario gameplay loop is active
	exit_game = false;								    // Indicates that the Mario gameplay loop is active
	std::string currFileName;
	int currLevelScore=0;
	int prevLevelsScore = 0;
	for (int i = screen_index; (i < files_names_vec.size()) && (playing_mario && !exit_game); i++)
	{
		valid_file = board.load(files_names_vec[i]);
		if (!valid_file)
		{
			loose();
			return;
		}
		
			std::string filename_prefix = files_names_vec[i].substr(0, files_names_vec[i].find_last_of('.'));
			std::string stepsFilename = filename_prefix + ".steps";
			std::string resultsFilename = filename_prefix + ".result";
		if (isSaveMode) {
			steps.clearSteps();
			results.clearResults();
			random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
			srand(static_cast<long>(random_seed)); // Seed the random number generator
			steps.setRandomSeed(random_seed);
		}

		prevLevelsScore = board.getLegendScore();  // Accumulate total score across levels; 
		playing_mario = true;					   // Indicates that the Mario gameplay loop is active
		exit_game = false;						   // Indicates that the Mario gameplay loop is active
		iteration = 0;
		ShowConsoleCursor(false);
		board.reset();
		board.printScreen(board.getGameBoard());
		mario.setStartingMario();
		initializeGhosts();
		shouldUpdateLegend = true;
		if (i == 0) 
			mario.resetLives();
		exit_game = false;

		char key = '\0';
		while (mario.getLives() != 0 && !exit_game) {
			size_t nextDiedIteration = 0;
			currIterationLostLife = false;
			mario.drawMario(); 
			
			isCollectingHammer();
			updateLegendState();


			if (mario.getIfWon() && i + 1 == files_names_vec.size())
				playing_mario = false;
			if (wonTheLevel())
			{
				break;
			}
			if (_kbhit())
			{
				key = std::tolower(_getch());
				updateAction(key);
			}
			checkAndUseHammer();

			Sleep(150);

			barrelManager.eraseBarrels(); //check
			ghostManager.eraseGhosts();
			mario.eraseMario();



			barrelManager.manageBarrelSpawning();
			mario.move();
			barrelManager.updateBarrels();
			ghostManager.updateGhosts();


			barrelManager.drawBarrels();
			ghostManager.drawGhosts();

			updateIfDiedByBarrel();
			updateIfDiedByGhost();
			barrelManager.clearDeadBarrels();
			checkIfMarioFellFromHigh();
			checkIfOffLimits();
			if (currIterationLostLife == true&&isSaveMode) {
				results.addResult(iteration, Results::died);
			}
			iteration++;
		}
		currLevelScore = board.getLegendScore() - prevLevelsScore;
		if (isSaveMode) {
			results.addResult(iteration, Results::finished);
			steps.saveSteps(stepsFilename);
			results.saveResults(resultsFilename,currLevelScore);
		}
		if (mario.getLives() == 0) 
		{ 
		loose(); 
		}
	}
}

void HumanInputGame::loose()
{
	resetScore();
	shouldUpdateLegend = true;
	mario.resetLives();
	board.printScreen(board.getLoosingBoard());
	Sleep(2000);
	board.resetHammerPosition();
	board.resetGhostPositions();
	board.resetSpecialGhostPositions();
	mario.setStartingMario();
	barrelManager.clearBarrels();
	ghostManager.clearGhosts();

	colorMode = false;
	setColor(GameCon::DEFAULT_COLOR); // Reset to default color
	board.setColorMode(false);
	board.printScreen(board.getStartBoard());
}
// Updates Mario's actions based on key presses from the user
void HumanInputGame::updateAction(int key)
{
	char ch;
	if (key == (int)GameCon::eKeys::EXIT_GAME)	 				// If the key corresponds to the "Exit Game" action
	{
		exit_game = true;
	}
	else if (key == (int)GameCon::eKeys::PAUSE)	 				// If the key corresponds to "Pause Game"
	{
		pauseGame();

	}
	else 
	{ 
		ch = mario.keyPressed(key);                                // For all other keys, pass the key to Mario's keyPressed handler
		if (ch != '\0') 
		{
			steps.addStep(iteration, ch);
		}
	}	
}







