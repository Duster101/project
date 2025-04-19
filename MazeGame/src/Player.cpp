// Player.cpp: Implementation of the Player class, handling player movement and interactions.

#include "Player.hpp"

// Moves the player based on input, updating position, score, and moves.
// @param dir: Direction to move ('W', 'A', 'S', 'D').
// @param grid: Reference to the game grid for collision and collectible checks.
void Player::move(char dir, std::vector<std::vector<char>>& grid) {
    Position newPos = pos; // Calculate new position based on current position.
    // Update new position based on direction.
    switch (dir) {
        case 'W': newPos.y--; break; // Move up.
        case 'A': newPos.x--; break; // Move left.
        case 'S': newPos.y++; break; // Move down.
        case 'D': newPos.x++; break; // Move right.
        default: return; // Ignore invalid input.
    }

    // Check if new position is within grid bounds.
    if (newPos.x >= 0 && newPos.x < grid[0].size() && 
        newPos.y >= 0 && newPos.y < grid.size()) {
        char cell = grid[newPos.y][newPos.x];
        // Allow movement if not a wall.
        if (cell != '#') {
            pos = newPos; // Update player position.
            moves++; // Increment move counter.
            if (cell == '*') {
                score += 10; // Add points for collecting item.
                grid[newPos.y][newPos.x] = ' '; // Remove collectible from grid.
            }
        }
    }
}