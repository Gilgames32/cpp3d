#if !defined(GAMELOGIC_H)
#define GAMELOGIC_H

#include "iostream"

#include "vector2.h"
#include "matrix.h"
#include "raycast.h"
#include "dintomb.hpp"

#include "memtrace.h"


class Entity
{
public:
    int id;
    Vector2 pos;
    Entity(int id = 0, const Vector2& pos = Vector2());
    Entity(const Entity&);
    void operator=(const Entity&);
    //~Entity();

    void Move(const Matrix &grid, const Vector2 moveDir, double deltaTime, double speed = 1);
};

class Player : public Entity
{
private:
    int health = 1000;
    double shootCoolDown = 0;
    double damageCoolDown = 0;
public:
    Vector2 dir;
    Vector2 GetPlane() const;

    Player(const Vector2& position = Vector2(), const Vector2& direction = Vector2(0, 1));
    Player(const Player &);
    bool Shoot(const Matrix &level, DinTomb<Entity>& entities);
    void DecreaseCoolDowns(double deltaTime);
    bool DamagePlayer(int damage);
};

class Input
{
protected:
    double turn;
    bool shootTrigger = false;
public:
    Vector2 dir;
    Input(const Vector2& dir = Vector2(), double turn = 0);
    //~Input();
    double GetTurn();
    bool GetShootTrigger();
};

class Game
{
public:
    Matrix level;
    Player player;
    //int entSize;
    //Entity *entities;
    DinTomb<Entity> ent;

    Game(const char* saveName);
    ~Game();

    bool SimulateGame(Input &inp, double deltaTime);
};

#endif // GAMELOGIC_H
