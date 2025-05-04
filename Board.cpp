#include <conio.h>
#include "Board.h"


#include <cstdlib>

void clrscr() {
    system("cls"); // Clear screen for Windows
}

void Board::reset() {
    for (int i = 0; i < MAX_Y; i++) {
        memcpy(currentBoard[i], originalBoard[i], MAX_X + 1);
    }
}

// Function made by the lecturer and chatGPT 
void Board::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) const {
    if (!std::filesystem::exists(directory)) {
        std::cerr << "Directory does not exist: " << directory << std::endl;
        return;
    }

    if (!std::filesystem::is_directory(directory)) {
        std::cerr << "Path is not a directory: " << directory << std::endl;
        return;
    }
    try {
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
            auto filenameStr = entry.path().filename().string();
            if (filenameStr.substr(0, 6) == "dkong_" && entry.path().extension() == ".screen") { //maybe magic number 6
                std::cout << filenameStr << " added!!\n";
                vec_to_fill.push_back(filenameStr);
            }
        }
        std::sort(vec_to_fill.begin(), vec_to_fill.end());
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading directory: " << e.what() << std::endl;
    }
}

void Board::printScreen(const char screen[][MAX_X + 1]) const {
    if (isSilentMode)
        return;
    clrscr(); // Clear screen for Windows
    setColor(GameCon::DEFAULT_COLOR); // Reset to default color
    for (int i = 0; i < MAX_Y - 1; i++) {
        for (int j = 0; j < MAX_X; j++) {
            char currentChar = screen[i][j];

            if (colorMode) {  // If colorMode is on, apply color
                if (currentChar == 'Q') {
                    std::cout << DARK_GRAY << "Q";  // Walls 
                }
                else if (currentChar == '=') {
                    std::cout << CYAN << "=";  // Cyan for floors
                }

                else if (currentChar == '$') {
                    std::cout << PINK << "$";  // Yellow for Pauline (letters)
                }
                else if (currentChar == '*') {
                    std::cout << RED << "*";  // Yellow for Pauline (letters)
                }
                else {
                    std::cout << RESET_COLOR << currentChar;  // Default color for other characters
                }
            }
            else {  // When colorMode is off, print without colors
                std::cout << RESET_COLOR << currentChar;
            }
        }
        std::cout << std::endl;
    }

    // Handle the last line without extra newline after printing
    std::cout << screen[MAX_Y - 1];
}

bool Board::load(const std::string& filename) {
    int marioCount = 0;
    int paulineCount = 0;
    int donkeyCount = 0;
    int legendCount = 0;

    start_pos_mario = { 0, 0 };
    start_pos_pauline = { 0, 0 };
    start_pos_donkey = { 0, 0 };
    start_pos_hammer = { 0, 0 };
    start_pos_ghosts_vec.clear();  // Clear the ghost vector
    legend.pos_L = { 0, 0 };

    std::ifstream screen_file(filename);                // Open file
    if (!screen_file) {                                 // Check if opened correctly
        handleErrors(screen_file);
        return false;
    }
    int curr_row = 0;
    int curr_col = 0;
    char c;
    while (!screen_file.get(c).eof() && curr_row < MAX_Y) {
        if (c == '\n') {
            if (curr_col < MAX_X) {
                // add spaces for missing cols
#pragma warning(suppress : 4996) // to allow strcpy
                strcpy(originalBoard[curr_row] + curr_col, std::string(MAX_X - curr_col - 1, ' ').c_str());
            }
            ++curr_row;
            curr_col = 0;
            continue;
        }
        if (curr_col < MAX_X) {
            switch (c)
            {
            case '@':
                marioCount++;
                start_pos_mario = { curr_col, curr_row };
                break;
            case '&':
                donkeyCount++;
                start_pos_donkey = { curr_col, curr_row };
                break;
            case '$':
                paulineCount++;
                start_pos_pauline = { curr_col, curr_row };
                break;
            case 'p':
                start_pos_hammer = { curr_col, curr_row };
                break;
            case 'x':
                start_pos_ghosts_vec.push_back({ curr_col, curr_row });
                break;
            case 'X':
                start_pos_special_ghosts_vec.push_back({ curr_col, curr_row });
                break;
            case 'L':
                legend.pos_L = { curr_col, curr_row };
                setPositionsInLegend();
                legendCount++;
                break;
            }
            originalBoard[curr_row][curr_col++] = c;
        }
    }
    screen_file.close();
    if (!validateBoard(marioCount, paulineCount, donkeyCount, legendCount))
    {
        Sleep(1500);
        return false;
    }
    return true;
}

void Board::handleErrors(const std::ifstream& _file) {

    clrscr();														// Clears the console screen before printing the new board
    if (_file.eof()) {
        std::cerr << "End of file reached." << std::endl;
    }
    else if (_file.fail()) {
        std::cerr << "Read error (failbit)." << std::endl;
    }
    else if (_file.bad()) {
        std::cerr << "Critical error (badbit)." << std::endl;
    }
    else {
        std::cerr << "Unknown error." << std::endl;
    }
    std::cout << "Returning to menu" << std::endl;
    Sleep(500);
}
void Board::setPositionsInLegend()
{
    int x = legend.pos_L.x;
    int y = legend.pos_L.y;
    legend.pos_score_in_legend = { x, y };
    legend.pos_life_in_legend = { x, y + 1 };
    legend.pos_hammer_in_legend = { x, y + 2 };
    legend.legendWidth = 10;  //maybe change here to kavua
    legend.legendHeight = 5;  //maybe change here to kavua
}


void Board::printLegend() const
{
    if (isSilentMode) {
        return;
    }
    if (legend.pos_L.x == 0 && legend.pos_L.y == 0)
    {
        return;
    }
    gotoxy(legend.pos_score_in_legend.x, legend.pos_score_in_legend.y);
    std::cout << legend.str_score << legend.score;

    gotoxy(legend.pos_life_in_legend.x, legend.pos_life_in_legend.y);
    std::cout << legend.str_life << legend.life;

    gotoxy(legend.pos_hammer_in_legend.x, legend.pos_hammer_in_legend.y);
    std::cout << legend.str_hammer << legend.hammer;
}

void Board::printScreenOptions(std::vector<std::string>& vec_to_fill) const
{
    clrscr();
    // Add headline - ascii art

    printScreen(chooseBoard_screen);

    if (vec_to_fill.empty())
    {
        printCenteredMessage(" No Files Founded To Read From");
        clrscr();
        printScreen(getStartBoard());
    }
    int x = 30; // MAGIC NUMBER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int y = 10;
    for (int i = 0; i < vec_to_fill.size(); i++)
    {
        gotoxy(x, y++);
        std::cout << "(" << i + 1 << ")  " << vec_to_fill[i] << std::endl;
    }
}


void Board::updateLegend(int score, int lives, bool hasHammer)
{

    legend.score = score;
    legend.life = lives;
    legend.hammer = hasHammer ? "ON " : "OFF";
    printLegend(); // Refresh the legend on the screen
}
void Board::clearLegend() const {
    int startX = legend.pos_L.x;
    int startY = legend.pos_L.y;
    int endX = startX + legend.legendWidth - 1;
    int endY = startY + legend.legendHeight - 1;
    if (isSilentMode)
        return;
    for (int y = startY; y <= endY; ++y) {
        gotoxy(startX, y);
        std::cout << std::string(legend.legendWidth, ' '); // Clear the entire row
    }

    std::cout.flush(); // Force refresh of console output
}

bool Board::validateBoard(int marioCount, int paulineCount, int donkeyCount, int legendCount)
{
    if (marioCount != 1) {
        clrscr();
        printCenteredMessage("Error: Mario ('@') must appear exactly once on the board.");
        return false;
    }
    if (paulineCount != 1) {
        clrscr();
        printCenteredMessage("Error: Pauline ('$') must appear exactly once on the board.");
        return false;
    }
    if (donkeyCount != 1) {
        clrscr();
        printCenteredMessage("Error: Donkey Kong('&') must appear exactly once on the board.");
        return false;
    }
    if (legendCount != 1) {
        clrscr();
        printCenteredMessage("Error: legend('L') must appear exactly once on the board.");
        return false;
    }
    for (auto& ghost : start_pos_ghosts_vec)
    {
        if (originalBoard[ghost.y + 1][ghost.x] != '=' &&
            originalBoard[ghost.y + 1][ghost.x] != '>' &&
            originalBoard[ghost.y + 1][ghost.x] != '<')
        {
            clrscr();
            printCenteredMessage("Error: ghost('x') must appear on floor.");
            return false;
        }

    }
    return true;
}

void Board::printCenteredMessage(const std::string& message) const
{
    int startCol = (MAX_X - message.length()) / 2;  // Horizontal center
    int startRow = MAX_Y / 2;  // Vertical center

    gotoxy(startCol, startRow);  // Move the cursor to the center position
    std::cout << message;  // Print the message
    Sleep(1500);
}


