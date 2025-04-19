// Player.hpp: Declaration of the Player class, managing player movement, score, and position.

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include <vector>

// Player class: Inherits from Entity, handles player movement and game interactions.
class Player : public Entity {
public:
    int score = 0; // Tracks player's score (increases by collecting items).
    int moves = 0; // Tracks number of moves made by the player.

    // Moves the player based on input, updating position, score, and moves.
    // @param dir: Direction to move ('W', 'A', 'S', 'D').
    // @param grid: Reference to the game grid for collision and collectible checks.
    void move(char dir, std::vector<std::vector<char>>& grid);
    
    // Pure virtual update function from Entity (not used, movement handled by move).
    void update() override {}
};

#endif