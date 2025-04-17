#ifndef ENTITY_HPP
#define ENTITY_HPP

struct Position {
    int x, y;
};

class Entity {
public:
    Position pos;
    virtual void update() = 0;
    virtual ~Entity() = default;
};

#endif
