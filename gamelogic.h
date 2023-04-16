#if !defined(GAMELOGIC_H)
#define GAMELOGIC_H

#include "vector2.h"
#include "matrix.h"
#include "raycast.h"

class Entity
{
public:
    Vector2 pos;
    Entity(Vector2 position = Vector2());
    //Entity(const Entity&);
    //~Entity();

    void Move(const Level& grid, Vector2 moveDir, double deltaTime);
};

class Player : public Entity
{
public:
    Vector2 dir;
    Vector2 plane() const;

    Player(Vector2 position = Vector2(), Vector2 direction = Vector2(1, 0));
    Player(const Player&);
};

class Input
{
protected:
    //
public:
    Vector2 dir;
    double turn;
    Input(Vector2 = Vector2(), double turn = 0);
    //~Input();
    double GetTurn();
};

class Game
{
public:
    Level gLevel;
    Player gPlayer;
    Entity *gEnemy;

    Game(Level gl = Level("palya.txt"), Player gp = Player(Vector2(2, 3)));
    //~Game();
    void SimulateGame(Input& inp, const double deltaTime);
};






#endif // GAMELOGIC_H
