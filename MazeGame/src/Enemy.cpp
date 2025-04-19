// Enemy.cpp: Implementation of the Enemy class, handling random enemy movement.

#include "Enemy.hpp"
#include <cstdlib>
#include <ctime>

// Constructor: Initializes enemy at the specified position.
// @param p: Starting position of the enemy.
Enemy::Enemy(Position p) { pos = p; }

// Updates enemy position by moving randomly, avoiding walls, exits, and collectibles.
// @param grid: Reference to the game grid for collision checks.
void Enemy::update(std::vector<std::vector<char>>& grid) {
    int direction = rand() % 4; // 0: Up, 1: Down, 2: Left, 3: Right
    Position newPos = pos; // Calculate new position.

    // Update new position based on random direction.
    switch (direction) {
        case 0: newPos.y--; break; // Move up.
        case 1: newPos.y++; break; // Move down.
        case 2: newPos.x--; break; // Move left.
        case 3: newPos.x++; break; // Move right.
    }

    // Check if new position is within bounds.
    if (newPos.x >= 0 && newPos.x < grid[0].size() && 
        newPos.y >= 0 && newPos.y < grid.size()) {
        char cell = grid[newPos.y][newPos.x];
        // Move only if destination is not a wall, exit, or collectible.
        if (cell != '#' && cell != 'E' && cell != '*') {
            pos = newPos; // Update enemy position.
        }
    }
}