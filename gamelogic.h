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
    int id;      /**< azonosító szám */
    Vector2 pos; /**< pozíció */

public:
    /**
     * alapértelmezett konstruktor
     * @param id azonosító szám
     * @param pos pozíció
     */
    Entity(int id = 0, const Vector2 &pos = Vector2());

    /**
     * azonosító szám gettere
     * @return azonosító szám
     */
    int GetID() const;

    /**
     * pozíció gettere
     * @return pozíció
     */
    const Vector2 &GetPos() const;

    /**
     * mozgatás
     * @param grid a pálya, amin mozgatunk
     * @param moveDir mozgatás iránya
     * @param deltaTime mozgatás időtartama
     * @param speed mozgás sebessége
     */
    void Move(const Matrix &grid, const Vector2 moveDir, double deltaTime, double speed = 1);
};

class Player : public Entity
{
private:
    int health = 100;          /**< életerő */
    double shootCoolDown = 0;  /**< visszaszámláló a következő érvényes lövésig  */
    double damageCoolDown = 0; /**< visszaszámláló a következő érvényes sebződésig */
    Vector2 dir;               /**< játékos nézésének iránya */

public:
    /**
     * életerő gettere
     * @return életerő
     */
    int GetHP() const;

    /**
     * nézés irányának gettere
     * @return nézés iránya
     */
    const Vector2 &GetDir() const;

    /**
     * nézés irányára merőleges vektor gettere
     * a vektor a látószög alapján van méretezve
     * @return nézés irányára merőleges vektor
     */
    Vector2 GetPlane() const;

    /**
     * alapértelmezett konstruktor
     * @param position pozíció
     * @param direction nézés kezdeti iránya
     */
    Player(const Vector2 &position = Vector2(), const Vector2 &direction = Vector2(0, 1));

    /**
     * másoló konstruktor
     */
    Player(const Player &);

    /**
     * forgatás
     * @param turn forgatás mértéke
     */
    void Rotate(double turn);

    /**
     * lövés
     * @param level a pálya, amin a lövés történik
     * @param entities entitások tömbje, akiket a lövés eltalálhat
     */
    bool Shoot(const Matrix &level, DinTomb<Entity> &entities);

    /**
     * a belső visszaszámlálókat visszaszámláló
     * @param deltaTime visszaszámolt idő
     */
    void DecreaseCoolDowns(double deltaTime);

    /**
     * játékos sebzése
     * @param damage sebzés mértéke
     * @return meghalt-e a sebzéstől a játékos
     */
    bool DamagePlayer(int damage);
};

class Input
{
protected:
    double turn;               /**< forgatás mértéke (egér mozgatás) */
    bool shootTrigger = false; /**< lövés kiváltó (egér kattintás) */
    Vector2 dir;               /**< mozgás iránya (wasd) */

public:
    /**
     * alapértelmezett konstruktor
     * @param dir mozgatás iránya
     * @param turn forgatás mértéke
     * @param shoot lövés kiváltó
     */
    Input(const Vector2 &dir = Vector2(), double turn = 0, bool shoot = false);

    /**
     * mozgatás irányának gettere
     * @return mozgatás iránya
     */
    const Vector2 &GetDir() const;

    /**
     * forgatás mértékének gettere
     * lekérés után nullázódik
     * @return forgatás mértéke
     */
    double GetTurn();

    /**
     * lövés kiváltó gettere
     * lekérés után nullázódik
     * @return lövés kiváltó
     */
    bool GetShootTrigger();
};

class Game
{
    Matrix level;             /**< pálya */
    Player player;            /**< játékos */
    DinTomb<Entity> entities; /**< entitások */

public:
    /**
     * konstruktor
     * @param saveName pálya fájljának neve
     */
    Game(const char *saveName);

    /**
     * pálya gettere
     * @return pálya
     */
    const Matrix &GetLevel() const;

    /**
     * játékos gettere
     * @return játékos
     */
    const Player &GetPlayer() const;

    /**
     * entitások gettere
     * @return entitások tömbje
     */
    const DinTomb<Entity> &GetEntities() const;

    /**
     * léptetés
     * adott bemenet és idő alapján kiszámítja a következő játékállást
     * @param inp bemenet
     * @param deltaTime léptetés ideje
     */
    bool SimulateGame(Input &inp, double deltaTime);
};

#endif // GAMELOGIC_H
