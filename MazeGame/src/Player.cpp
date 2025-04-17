#include "Player.hpp"

void Player::move(char dir, std::vector<std::vector<char>>& grid) {
    Position newPos = pos;
    switch (dir) {
        case 'W': newPos.y--; break;
        case 'A': newPos.x--; break;
        case 'S': newPos.y++; break;
        case 'D': newPos.x++; break;
        default: return;
    }

    // Check bounds and collisions
    if (newPos.x >= 0 && newPos.x < grid[0].size() && 
        newPos.y >= 0 && newPos.y < grid.size()) {
        char cell = grid[newPos.y][newPos.x];
        if (cell != '#') {
            pos = newPos;
            moves++;
            if (cell == '*') {
                score += 10; 
                grid[newPos.y][newPos.x] = ' '; 
            }
        }
    }
}