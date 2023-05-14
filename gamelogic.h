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
protected:
    int id;
    Vector2 pos;

public:
    Entity(int id = 0, const Vector2 &pos = Vector2());

    int GetID() const;
    const Vector2 &GetPos() const;

    void Move(const Matrix &grid, const Vector2 moveDir, double deltaTime, double speed = 1);
};

class Player : public Entity
{
private:
    int health = 100;
    double shootCoolDown = 0;
    double damageCoolDown = 0;
    Vector2 dir;

public:
    const Vector2 &GetDir() const;
    Vector2 GetPlane() const;

    Player(const Vector2 &position = Vector2(), const Vector2 &direction = Vector2(0, 1));
    Player(const Player &);

    void Rotate(double turn);
    bool Shoot(const Matrix &level, DinTomb<Entity> &entities);
    void DecreaseCoolDowns(double deltaTime);
    bool DamagePlayer(int damage);
};

class Input
{
protected:
    double turn;
    bool shootTrigger = false;
    Vector2 dir;

public:
    Input(const Vector2 &dir = Vector2(), double turn = 0);

    const Vector2 &GetDir() const;
    double GetTurn();
    bool GetShootTrigger();
};

class Game
{
    Matrix level;
    Player player;
    DinTomb<Entity> entities;

public:
    Game(const char *saveName);

    const Matrix &GetLevel() const;
    const Player &GetPlayer() const;
    const DinTomb<Entity> &GetEntities() const;

    bool SimulateGame(Input &inp, double deltaTime);
};

#endif // GAMELOGIC_H
