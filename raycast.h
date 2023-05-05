#if !defined(RAYCAST_H)
#define RAYCAST_H

#include "vector2.h"
#include "matrix.h"

class Ray
{
public:
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

    // a becsapódási cella értéke
    Duo<int> cell;

    // a pálya
    const Matrix& space;

    Ray(const Matrix& sapceGrid, const Vector2& startVector, const Vector2& dirVector);
    Ray(const Ray& ray);

    double WallX();

    int CellValue();

    //~Ray();
};

#endif // RAYCAST_H
