// Game.hpp: Header file for the Game class, managing the maze game state and logic

#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Enemy.hpp"
#include <vector>
#include <string>
#include <windows.h> 

// Game class: Manages the game grid, player, enemies, levels, and console output
class Game {
private:
    static const int SIZE = 10; // Grid size (10x10)
    std::vector<std::vector<char>> grid; // 2D grid storing maze layout
    Player player; // Player object for position and stats
    std::vector<Enemy> enemies; // List of enemies
    Position exit; // Exit position
    int currentLevel = 0; // Current level index (0 to 4)
    std::vector<std::string> levelFiles; // File paths for level data
    HANDLE hConsole; // Console handle for colored output

    // Sets console text color for rendering
    void setConsoleColor(int color) {
        SetConsoleTextAttribute(hConsole, color);
    }

public:
    Game(); // Constructor: Initializes game state and loads first level
    void loadLevel(const std::string& filename); // Loads maze from file
    void saveLevel(const std::string& filename); // Saves maze to file
    void render(); // Renders grid, player, enemies, and UI
    bool isGameOver(bool& win); // Checks win (exit) or lose (enemy collision)
    void gameLoop(); // Main game loop for input and updates
};

#endif