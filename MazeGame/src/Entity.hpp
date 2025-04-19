// Entity.hpp: Header file for the Entity base class, defining common properties for game objects
#ifndef ENTITY_HPP
#define ENTITY_HPP

// Position struct: Stores x, y coordinates for entities
struct Position {
    int x, y;
};

// Entity class: Abstract base class for game objects (Player, Enemy)
class Entity {
public:
    Position pos; // Current position in the grid
    // Pure virtual function for updating entity state
    virtual void update() = 0;
    // Virtual destructor for proper cleanup of derived classes
    virtual ~Entity() = default;
};

#endif