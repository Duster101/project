#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include <vector>

class Enemy : public Entity {
public:
    Enemy() = default;
    Enemy(Position p);
    void update(std::vector<std::vector<char>>& grid);
    void update() override {}
};

#endif
