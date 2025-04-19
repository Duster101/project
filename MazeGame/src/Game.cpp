// Game.cpp: Implementation of the Game class, managing the maze game logic, rendering, and level handling.

#include "Game.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

// Constructor: Initializes the game with random seed, console handle, and loads the first level.
Game::Game() {
    srand(time(0)); // Seed random number generator for enemy movement.
    levelFiles = {"levels/level1.txt", "levels/level2.txt", "levels/level3.txt", "levels/level4.txt", "levels/level5.txt"};
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle for color and cursor control.
    loadLevel(levelFiles[currentLevel]); // Load initial level.
}

// Loads a level from a text file, populating the grid and setting player, enemies, and exit positions.
// @param filename: Path to the level file (e.g., "levels/level1.txt").
void Game::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        // Handle file open failure.
        std::cerr << "Failed to load level: " << filename << std::endl;
        std::cerr << "Press any key to exit..." << std::endl;
        _getch();
        ::exit(1);
    }

    grid.clear(); // Clear existing grid.
    enemies.clear(); // Clear existing enemies.
    player.score = 0; // Reset player score.
    player.moves = 0; // Reset player moves.
    std::string line;
    // Read file line by line to build the grid.
    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }

    bool foundPlayer = false;
    // Scan grid to set player, exit, and enemy positions.
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == 'P') {
                player.pos = {x, y}; // Set player position.
                foundPlayer = true;
            }
            if (grid[y][x] == 'E') exit = {x, y}; // Set exit position.
            if (grid[y][x] == 'X') enemies.push_back(Enemy({x, y})); // Add enemy at position.
        }
    }

    // Ensure player position is defined in the level.
    if (!foundPlayer) {
        std::cerr << "Error: Player position ('P') not found in " << filename << std::endl;
        std::cerr << "Press any key to exit..." << std::endl;
        _getch();
        ::exit(1);
    }

    file.close(); // Close the file.
}

// Saves the current grid to a text file.
// @param filename: Path to save the level file.
void Game::saveLevel(const std::string& filename) {
    std::ofstream file(filename);
    // Write each row of the grid to the file.
    for (const auto& row : grid) {
        for (char c : row) file << c;
        file << '\n';
    }
}

// Renders the game grid, player, enemies, and UI (score, moves, level) using console output.
void Game::render() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hConsole, coord); // Reset cursor to top-left.

    // Check for empty grid to prevent crashes.
    if (grid.empty()) {
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "Error: Grid is empty!\n";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    // Draw top border.
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "  +--------------------+\n";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Render each grid cell with appropriate characters and colors.
    for (int y = 0; y < grid.size(); y++) {
        std::cout << "  |";
        for (int x = 0; x < grid[y].size(); x++) {
            if (player.pos.y == y && player.pos.x == x) {
                // Draw player in green.
                setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << 'P';
                continue;
            }
            bool enemyHere = false;
            // Check for enemies at this position.
            for (const auto& enemy : enemies) {
                if (enemy.pos.y == y && enemy.pos.x == x) {
                    enemyHere = true;
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    std::cout << 'X';
                    break;
                }
            }
            if (!enemyHere) {
                char c = grid[y][x];
                if (c == '#') {
                    // Draw walls in default color.
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    std::cout << c;
                } else if (c == '*') {
                    // Draw collectibles in yellow.
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    std::cout << c;
                } else if (c == 'E') {
                    // Draw exit in cyan.
                    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    std::cout << c;
                } else {
                    // Draw paths in default color.
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    std::cout << c;
                }
            }
        }
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << "|\n";
    }

    // Draw bottom border.
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "  +--------------------+\n";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Display score, moves, and level.
    std::cout << "  Score: ";
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << player.score;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << " | Moves: ";
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << player.moves;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << " | Level: ";
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << (currentLevel + 1);
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "/5\n";

    // Show control instructions.
    std::cout << "  Use WASD to move, Q to quit\n";
}

// Checks if the game is over (win: player reaches exit; lose: player hits enemy).
// @param win: Set to true if player reaches exit, false if caught by enemy.
// @return True if game is over, false otherwise.
bool Game::isGameOver(bool& win) {
    // Check win condition: player reaches exit.
    if (player.pos.x == exit.x && player.pos.y == exit.y) {
        win = true;
        return true;
    }

    // Check lose condition: player collides with any enemy.
    for (const auto& enemy : enemies) {
        if (player.pos.x == enemy.pos.x && player.pos.y == enemy.pos.y) {
            win = false;
            return true;
        }
    }
    return false;
}

// Main game loop: Handles level progression, input, updates, and game over states.
void Game::gameLoop() {
    // Continue until all levels are completed or game ends.
    while (currentLevel < levelFiles.size()) {
        loadLevel(levelFiles[currentLevel]); // Load current level.
        bool win = false;
        render(); // Initial render.
        // Inner loop: Run until game over (win or lose).
        while (!isGameOver(win)) {
            // Update all enemies.
            for (auto& enemy : enemies) {
                enemy.update(grid);
            }
            // Check game over after enemy updates.
            if (isGameOver(win)) {
                render();
                break;
            }
            char input = _getch(); // Get user input.
            if (input == 'q' || input == 'Q') {
                // Handle quit game.
                COORD coord = {0, 0};
                SetConsoleCursorPosition(hConsole, coord);
                setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << "  +--------------------+\n";
                std::cout << "  |      QUITTING      |\n";
                std::cout << "  +--------------------+\n";
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << "  Final Score: " << player.score << "\n";
                std::cout << "  Total Moves: " << player.moves << "\n";
                std::cout << "  Press any key to exit...\n";
                _getch();
                return;
            }
            grid[player.pos.y][player.pos.x] = ' '; // Clear current player position.
            player.move(toupper(input), grid); // Move player based on input.
            grid[player.pos.y][player.pos.x] = 'P'; // Update player position on grid.
            // Check game over after player move.
            if (isGameOver(win)) {
                render();
                break;
            }
            render(); // Redraw game state.
        }
        if (win) {
            // Handle level completion.
            COORD coord = {0, static_cast<SHORT>(grid.size() + 4)};
            SetConsoleCursorPosition(hConsole, coord);
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "  +--------------------+\n";
            std::cout << "  |  LEVEL " << (currentLevel + 1) << " COMPLETED  |\n";
            std::cout << "  +--------------------+\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "  Score: " << player.score << " | Moves: " << player.moves << "\n";
            currentLevel++; // Advance to next level.
            if (currentLevel < levelFiles.size()) {
                std::cout << "  Loading next level...\n";
            } else {
                // Handle game completion (all levels done).
                setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << "  +--------------------+\n";
                std::cout << "  |    YOU WON THE     |\n";
                std::cout << "  |       GAME!        |\n";
                std::cout << "  +--------------------+\n";
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << "  Final Score: " << player.score << "\n";
                std::cout << "  Total Moves: " << player.moves << "\n";
            }
        } else {
            // Handle game over (player caught by enemy).
            COORD coord = {0, static_cast<SHORT>(grid.size() + 4)};
            SetConsoleCursorPosition(hConsole, coord);
            setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "  +--------------------+\n";
            std::cout << "  |      GAME OVER     |\n";
            std::cout << "  +--------------------+\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "  You were caught by an enemy!\n";
            std::cout << "  Final Score: " << player.score << "\n";
            std::cout << "  Total Moves: " << player.moves << "\n";
            break; // Exit outer loop to end game.
        }
        std::cout << "  Press any key to continue...\n";
        _getch(); // Wait for user input before next level or exit.
    }
    system("pause"); // Pause console before closing.
}