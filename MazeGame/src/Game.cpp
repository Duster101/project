#include "Game.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

Game::Game() {
    srand(time(0));
    levelFiles = {"levels/level1.txt", "levels/level2.txt", "levels/level3.txt", "levels/level4.txt", "levels/level5.txt"};
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    loadLevel(levelFiles[currentLevel]);
}

void Game::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to load level: " << filename << std::endl;
        std::cerr << "Press any key to exit..." << std::endl;
        _getch();
        ::exit(1);
    }

    grid.clear();
    enemies.clear();
    player.score = 0;
    player.moves = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }

    bool foundPlayer = false;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == 'P') {
                player.pos = {x, y};
                foundPlayer = true;
            }
            if (grid[y][x] == 'E') exit = {x, y};
            if (grid[y][x] == 'X') enemies.push_back(Enemy({x, y}));
        }
    }

    if (!foundPlayer) {
        std::cerr << "Error: Player position ('P') not found in " << filename << std::endl;
        std::cerr << "Press any key to exit..." << std::endl;
        _getch();
        ::exit(1);
    }

    file.close();
}

void Game::saveLevel(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& row : grid) {
        for (char c : row) file << c;
        file << '\n';
    }
}

void Game::render() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hConsole, coord);

    if (grid.empty()) {
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "Error: Grid is empty!\n";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "  +--------------------+\n";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    for (int y = 0; y < grid.size(); y++) {
        std::cout << "  |";
        for (int x = 0; x < grid[y].size(); x++) {
            if (player.pos.y == y && player.pos.x == x) {
                setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << 'P';
                continue;
            }
            bool enemyHere = false;
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
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    std::cout << c;
                } else if (c == '*') {
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    std::cout << c;
                } else if (c == 'E') {
                    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    std::cout << c;
                } else {
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    std::cout << c;
                }
            }
        }
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << "|\n";
    }

    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "  +--------------------+\n";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

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

    std::cout << "  Use WASD to move, Q to quit\n";
}

bool Game::isGameOver(bool& win) {
    if (player.pos.x == exit.x && player.pos.y == exit.y) {
        win = true;
        return true;
    }

    for (const auto& enemy : enemies) {
        if (player.pos.x == enemy.pos.x && player.pos.y == enemy.pos.y) {
            win = false;
            return true;
        }
    }
    return false;
}

void Game::gameLoop() {
    while (currentLevel < levelFiles.size()) {
        loadLevel(levelFiles[currentLevel]);
        bool win = false;
        render();
        while (!isGameOver(win)) {
            for (auto& enemy : enemies) {
                enemy.update(grid);
            }
            if (isGameOver(win)) {
                render();
                break;
            }
            char input = _getch();
            if (input == 'q' || input == 'Q') {
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
            grid[player.pos.y][player.pos.x] = ' ';
            player.move(toupper(input), grid);
            grid[player.pos.y][player.pos.x] = 'P';
            if (isGameOver(win)) {
                render();
                break;
            }
            render();
        }
        if (win) {
            COORD coord = {0, static_cast<SHORT>(grid.size() + 4)};
            SetConsoleCursorPosition(hConsole, coord);
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "  +--------------------+\n";
            std::cout << "  |  LEVEL " << (currentLevel + 1) << " COMPLETED  |\n";
            std::cout << "  +--------------------+\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "  Score: " << player.score << " | Moves: " << player.moves << "\n";
            currentLevel++;
            if (currentLevel < levelFiles.size()) {
                std::cout << "  Loading next level...\n";
            } else {
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
            break; // Exit the outer loop to end the game
        }
        std::cout << "  Press any key to continue...\n";
        _getch();
    }
    system("pause");
}