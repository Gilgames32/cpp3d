#if !defined(RAYCAST_H)
#define RAYCAST_H

#include "iostream"

#include "vector2.h"
#include "matrix.h"

#include "memtrace.h"

class Ray
{
    const Vector2 start; /**< kiindulási pont koordinátavektora */
    Vector2 end;         /**< becsapódási pont koordinátavektora */

    const Vector2 dir; /**< a sugár irányvektora */

    double wallDist; /**< halszemeffektusmentes távolság */
                     /**< egy dir-re merőleges egyenestől való távolsága a becsapódásnak */

    bool side; /**< becsapódási oldal */
               /**< ÉD vagy NYK oldalt talált el a sugár*/

    int cellValue; /**< becsapódási cella értéke */

public:
    /**
     * konstruktor és a sugár kiszámítása
     * kezdeti értékek szerint DDA-val kiszámítja a sugarat
     * @param spaceGrid a pálya, ahol a sugarat vetjük
     * @param startVector kiindulási pont
     * @param dirVector sugár iránya
     */
    Ray(const Matrix &sapceGrid, const Vector2 &startVector, const Vector2 &dirVector);

    /**
     * másoló konstruktor
     */
    Ray(const Ray &ray);

    /**
     * @return a cella falát hol találta el (0 és 1 közti arányszám)
     */
    double WallX() const;

    /**
     * cellaérték gettere
     * @return a becsapódás cellájának értéke
     */
    int CellValue() const;

    /**
     * kamera síkjával párhuzamos egyenes és a becsapódási pont (merőleges) távolságának gettere
     * @return fal távolsága
     */
    double GetWallDist() const;

    /**
     * becsapódási oldal gettere
     * @return becsapódási oldal
     */
    bool GetSide() const;

    /**
     * kiindulási pont gettere
     * @return kiindulási pont
     */
    const Vector2 &GetStart() const;

    /**
     * becsapódási pont gettere
     * @return becsapódási pont
     */
    const Vector2 &GetEnd() const;
};

#endif // RAYCAST_H
