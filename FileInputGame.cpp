#include "FileInputGame.h"
#include "Windows.h"

// Manages the overall flow of the game
void FileInputGame::run()
{
	
	board.setSilentMode(isSilentMode);
	system("mode con cols=80 lines=25");			// Set the console size to be 80X25
	ShowConsoleCursor(false);						// Hides the console cursor to improve visual appearance during the game

	board.getAllBoardFileNames(files_names_vec);

	bool playing = true;

	while (playing) {
		startGame(0);
		playing = false;
	}
}


void FileInputGame::startGame(int screen_index) {
	bool valid_file;
	playing_mario = true;							    // Indicates that the Mario gameplay loop is active
	exit_game = false;								    // Indicates that the Mario gameplay loop is active

	for (int i = screen_index; (i < files_names_vec.size()) && (playing_mario && !exit_game); i++)
	{
		valid_file = board.load(files_names_vec[i]);
		if (!valid_file)
		{
			loose();
			return;
		}

		currFileName = files_names_vec[i];
		std::string filename_prefix = files_names_vec[i].substr(0, files_names_vec[i].find_last_of('.'));
		std::string stepsFilename = filename_prefix + ".steps";
		std::string resultsFilename = filename_prefix + ".result";

		steps= Steps::loadSteps(stepsFilename);
		random_seed = steps.getRandomSeed();
		results=Results::loadResults(resultsFilename); 
		srand(random_seed);

		playing_mario = true;							// Indicates that the Mario gameplay loop is active
		exit_game = false;								// Indicates that the Mario gameplay loop is active
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
			nextDiedIteration = 0;
			currIterationLostLife = false;
			if (wonTheLevel())
			{
				break;
			}
			// check if the result is correct
			if (results.isFinishedBy(iteration)) {
				reportResultError("Results file reached finish while game hadn't!", files_names_vec[i], iteration);
				unmatching_result_found = true;
				break;
			}
			else {
				nextDiedIteration = results.getNextDeathIteration();
			}

			mario.drawMario();
			isCollectingHammer();
			updateLegendState();


			if (mario.getIfWon() && i + 1 == files_names_vec.size()) {
				playing_mario = false;
				break;
			}
			if (wonTheLevel())
				break;
			if (steps.isNextStepOnIteration(iteration))
			{
				key = steps.popStep();
				updateAction(key);
			}
			checkAndUseHammer();

			Sleep(isSilentMode ? 0:50);

			barrelManager.eraseBarrels(); 
			ghostManager.eraseGhosts();
			mario.eraseMario();



			barrelManager.manageBarrelSpawning();
			mario.move();
			barrelManager.updateBarrels();
			ghostManager.updateGhosts();



			barrelManager.drawBarrels();

			updateIfDiedByBarrel();
			updateIfDiedByGhost();
			barrelManager.clearDeadBarrels();
			checkIfMarioFellFromHigh();
			checkIfOffLimits();

			if (currIterationLostLife == true) {
				// check if the result is correct
				if (results.popResult() != std::pair{ iteration, Results::died }) {
					reportResultError("Results file doesn't match hit bomb event!", currFileName, iteration);
					unmatching_result_found = true;
					break;
				}
				nextDiedIteration = results.getNextDeathIteration();
			}
			else {
				// we didn't hit a bomb, so we check if the result is correct
				if (iteration == nextDiedIteration) {
					reportResultError("Results file has a hit bomb event that didn't happen!", currFileName, iteration);
					unmatching_result_found = true;
					break;
				}
			}
			iteration++;
		}
		// check if the result is correct, but only if we didn't find an unmatching result already
		if (!unmatching_result_found) {
			if (results.popResult() != std::pair{ iteration, Results::finished }) {
				reportResultError("Results file doesn't match finished event!", currFileName, iteration);
				unmatching_result_found = true;
			}
			if (results.popResult().second != Results::noResult) {
				reportResultError("Results file has additional events after finish event!", currFileName, iteration);
				unmatching_result_found = true;
			}
		}
	}
}

void FileInputGame::loose()
{
	system("cls");
	std::cout << "Mario died before finishing the 3rd screen ";
}

// Updates Mario's actions based on key presses from the user
void FileInputGame::updateAction(int key)
{
	mario.keyPressed(key);	// pass the key to Mario's keyPressed handler
}


void FileInputGame::reportResultError(const std::string& message, const std::string& filename, size_t iteration) {
	system("cls");
	std::cout << "Screen " << filename << " - " << message << '\n';
	std::cout << "Iteration: " << iteration << '\n';
	std::cout << "Press any key to continue to next screens (if any)" << std::endl;
	_getch();
}
