#if !defined(GAMELOGIC_H)
#define GAMELOGIC_H

#include "vector2.h"

class Player
{
    Vector2 pos;
    Vector2 dir;
public:
    Player(/* args */);
    ~Player();
};

class Level
{
private:
    int **grid;
public:
    Level(/* args */);
    ~Level();
};




#endif // GAMELOGIC_H
