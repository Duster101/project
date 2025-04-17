#include "Enemy.hpp"
#include <cstdlib>
#include <ctime>

Enemy::Enemy(Position p) { pos = p; }

void Enemy::update(std::vector<std::vector<char>>& grid) {
    int direction = rand() % 4; // 0: Up, 1: Down, 2: Left, 3: Right
    Position newPos = pos;

    switch (direction) {
        case 0: newPos.y--; break; // for Up
        case 1: newPos.y++; break; // for Down
        case 2: newPos.x--; break; // for Left
        case 3: newPos.x++; break; // for Right
    }

    // Check bounds and collisions
    if (newPos.x >= 0 && newPos.x < grid[0].size() && 
        newPos.y >= 0 && newPos.y < grid.size()) {
        char cell = grid[newPos.y][newPos.x];
        if (cell != '#' && cell != 'E' && cell != '*') { // Avoid walls, exit, and collectibles
            pos = newPos;
        }
    }
}