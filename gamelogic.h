#if !defined(GAMELOGIC_H)
#define GAMELOGIC_H

#include "vector2.h"
#include "matrix.h"
#include "raycast.h"

class Entity
{
public:
    int id;
    Vector2 pos;
    Entity(int id = 0, const Vector2& pos = Vector2());
    // Entity(const Entity&);
    //~Entity();

    void Move(const Matrix &grid, const Vector2 moveDir, double deltaTime);
};

class Player : public Entity
{
public:
    Vector2 dir;
    Vector2 plane() const;

    Player(const Vector2& position = Vector2(), const Vector2& direction = Vector2(0, 1));
    Player(const Player &);
};

class Input
{
protected:
    //
public:
    Vector2 dir;
    double turn;
    Input(const Vector2& dir = Vector2(), double turn = 0);
    //~Input();
    double GetTurn();
};

class Game
{
public:
    Matrix level;
    Player player;
    int entSize;
    Entity *entities;

    Game(const char* saveName);
    //~Game();
    void SimulateGame(Input &inp, double deltaTime);
};

#endif // GAMELOGIC_H
