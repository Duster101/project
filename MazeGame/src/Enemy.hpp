// Enemy.hpp: Declaration of the Enemy class, representing enemies that move randomly in the maze game.

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include <vector>

// Enemy class: Inherits from Entity, defines enemy behavior and position.
class Enemy : public Entity {
public:
    // Default constructor: Initializes an enemy with default position.
    Enemy() = default;
    
    // Constructor: Initializes an enemy at the specified position.
    // @param p: Starting position of the enemy.
    Enemy(Position p);
    
    // Updates enemy position by moving randomly, avoiding walls, exits, and collectibles.
    // @param grid: Reference to the game grid for collision checks.
    void update(std::vector<std::vector<char>>& grid);
    
    // Pure virtual update function from Entity (not used, overridden by specific update).
    void update() override {}
};

#endif