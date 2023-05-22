#include "raycast.h"

Ray::Ray(const Matrix &spaceGrid, const Vector2 &startVector, const Vector2 &dirVector)
    : start(startVector), dir(dirVector)
{
    // kiindulási cella
    Duo<int> cell(start.x, start.y);

    // egyik x vagy y oldalról a legközelebbi átellenes oldalig a távolság
    // 1 helyett átfogó kéne
    Vector2 deltaDist = Vector2(abs(1 / dir.x), abs(1 / dir.y));

    // lépegetés irányának előjele, -1 vagy 1
    Duo<int> stepDir;
    stepDir.x = dir.x < 0 ? -1 : 1;
    stepDir.y = dir.y < 0 ? -1 : 1;

    // rácsvonalanként léptetett pont
    // starthoz relatív
    Vector2 sideDist;
    // legközelebbi falig sugár
    // mert nem rácsponton állunk
    sideDist.x = (dir.x < 0 ? start.x - cell.x : cell.x + 1.0 - start.x) * deltaDist.x;
    sideDist.y = (dir.y < 0 ? start.y - cell.y : cell.y + 1.0 - start.y) * deltaDist.y;

    // ameddig tart a bor addig megyek
    // tbh lövésem sincs mit csinál ha out of bounds vagyunk lol
    // spicy oneliner :3
    while (!(cell.x < 0 || cell.y < 0 || cell.x >= spaceGrid.GetSize().x || cell.y >= spaceGrid.GetSize().y) && spaceGrid[cell.x][cell.y] == 0)
    {
        // DDA algoritmus
        if (sideDist.x < sideDist.y)
        {
            sideDist.x += deltaDist.x;
            cell.x += stepDir.x;
            side = false;
        }
        else
        {
            sideDist.y += deltaDist.y;
            cell.y += stepDir.y;
            side = true;
        }
    }

    // egy kis matek
    wallDist = side == 0 ? sideDist.x - deltaDist.x : sideDist.y - deltaDist.y;

    end = start + dir * wallDist;
}

// deszépvagy egyemzuzádat
Ray::Ray(const Ray &ray) : start(ray.start),
                           end(ray.end),
                           dir(ray.dir),
                           wallDist(ray.wallDist),
                           side(ray.side)
{}

double Ray::WallX() const
{
    double wallX;
    if (side == 0)
        wallX = start.y + wallDist * dir.y;
    else
        wallX = start.x + wallDist * dir.x;
    wallX -= floor(wallX);
    return wallX;
}

// trust me van értelme gettert írni mindenhez :|

int Ray::CellValue() const { return cellValue; }

double Ray::GetWallDist() const { return wallDist; }

bool Ray::GetSide() const { return side; }

const Vector2 &Ray::GetStart() const { return start; }

const Vector2 &Ray::GetEnd() const { return end; }
