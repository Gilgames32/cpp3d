#if !defined(GAMELOGIC_H)
#define GAMELOGIC_H

#include "vector2.h"
#include <iostream>
#include <fstream>

class Player
{
public:
    Vector2 pos;
    Vector2 dir;
    Vector2 plane() const;
    Player(Vector2 position = Vector2(), Vector2 direction = Vector2(1, 0));
};

class Level
{
public:
    int sizex, sizey;
    int **grid;
    
    Level(const char* fileName);
    //Level(const Level &l);
    ~Level();

    int* operator[](size_t index) const;
};




#endif // GAMELOGIC_H
