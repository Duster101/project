#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include <vector>

class Player : public Entity {
public:
    int score = 0;
    int moves = 0;

    void move(char dir, std::vector<std::vector<char>>& grid);
    void update() override {}
};

#endif
