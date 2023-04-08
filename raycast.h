#if !defined(RAYCAST_H)
#define RAYCAST_H

#include "vector2.h"
#include "gamelogic.h"

class Ray
{
public:
    // kiindulási és végkoordináta
    Vector2 start, end;

    // a sugár irányvektora
    Vector2 dir;

    // rácsvonalanként léptetett pont
    // starthoz relatív
    Vector2 sideDist;

    // halszemeffektusmentes távolság
    // azaz egy dir-re merőleges egyenestől való távolsága a becsapódásnak
    double wallDist;

    // a becsapódási oldal
    bool side;

    Ray(const Level& sapce, const Vector2& startVector, const Vector2& dirVector);
    Ray(const Ray& ray);
    //~Ray();
};

#endif // RAYCAST_H
