#if !defined(RAYCAST_H)
#define RAYCAST_H

#include "iostream"

#include "vector2.h"
#include "matrix.h"

#include "memtrace.h"

class Ray
{
    // kiindulási és végkoordináta
    const Vector2 start;
    Vector2 end;

    // a sugár irányvektora
    const Vector2 dir;

    // rácsvonalanként léptetett pont
    // starthoz relatív
    Vector2 sideDist;

    // halszemeffektusmentes távolság
    // azaz egy dir-re merőleges egyenestől való távolsága a becsapódásnak
    double wallDist;

    // a becsapódási oldal
    bool side;

    // a becsapódási cella koordinátája
    Duo<int> cell;

    // a pálya
    const Matrix &space;

public:
    Ray(const Matrix &sapceGrid, const Vector2 &startVector, const Vector2 &dirVector);
    Ray(const Ray &ray);

    // a fal cellát pontosan hol találta el 0...1
    double WallX() const;

    int CellValue() const;

    double GetWallDist() const;
    bool GetSide() const;
    const Vector2 &GetStart() const;
    const Vector2 &GetEnd() const;

    //~Ray();
};

#endif // RAYCAST_H
