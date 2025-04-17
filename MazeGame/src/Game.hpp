#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Enemy.hpp"
#include <vector>
#include <string>
#include <windows.h> 

class Game {
private:
    static const int SIZE = 10;
    std::vector<std::vector<char>> grid;
    Player player;
    std::vector<Enemy> enemies;
    Position exit;
    int currentLevel = 0;
    std::vector<std::string> levelFiles;
    HANDLE hConsole; 

    void setConsoleColor(int color) {
        SetConsoleTextAttribute(hConsole, color);
    }

public:
    Game();
    void loadLevel(const std::string& filename);
    void saveLevel(const std::string& filename);
    void render();
    bool isGameOver(bool& win);
    void gameLoop();
};

#endif