#pragma once
class GameCon
{
public:
    enum class eMoves { LEFT = 'a', RIGHT = 'd', UP = 'w', DOWN = 'x', STAY = 'S' };
    enum class eKeys { START_NEW_GAME = '1', INSTRUCTIONS = '8', EXIT_GAME = '9', PAUSE = 27, RETURN_BACK = 'r', TOGGLE_COLORS = 'c' };
    static constexpr int DEAD_MARIO = 0;
    static constexpr int FULL_LIVES = 3;
    static constexpr int EXPLOSION_RADIUS = 2;
    static constexpr int GAME_WIDTH = 80;
    static constexpr int GAME_HEIGHT = 25;

    static constexpr int DEFAULT_COLOR = 7;  // White
    static constexpr int COLOR_MARIO = 12;  // Red
    static constexpr int COLOR_BARREL = 6;  // Bright Brown
    static constexpr int COLOR_GHOST = 14;  // Bright Yellow
    static constexpr int COLOR_FLOOR = 11;  // Cyan
    static constexpr int COLOR_LADDER = 10; // Green or another complementary color




    struct Direction {
        int x, y; 
        bool operator==(const Direction& other) const {
            return(x == other.x && y == other.y);
        }
        bool operator!=(const Direction& other) const {
            return(x != other.x || y != other.y);
        }
    };
    struct Position {
        int x = 0;
        int y = 0;


    }; 
    static constexpr Direction directions[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0} };
    enum class eDir { UP, LEFT, DOWN, RIGHT, STAY };
    static constexpr char PAULINE = '$';
    static constexpr char DONKEYKONG = '@';

    static constexpr int MIN_X = 10;
    static constexpr int MIN_Y = 3;


};