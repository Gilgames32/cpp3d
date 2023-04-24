#include "raycast.h"

Ray::Ray(const Matrix &spaceGrid, const Vector2 &startVector, const Vector2 &dirVector)
    : space(spaceGrid), start(startVector), dir(dirVector)
{
    // kiindulási cella
    cell = pair<int>(start.x, start.y);

    // egyik x vagy y oldalról a legközelebbi átellenes oldalig a távolság
    // 1 helyett átfogó kéne
    Vector2 deltaDist = Vector2(abs(1 / dir.x), abs(1 / dir.y));

    // lépegetés irányának előjele, -1 vagy 1
    pair<int> stepDir;
    stepDir.x = dir.x < 0 ? -1 : 1;
    stepDir.y = dir.y < 0 ? -1 : 1;

    // legközelebbi falig sugár
    // mert nem rácsponton állunk
    sideDist.x = (dir.x < 0 ? start.x - cell.x : cell.x + 1.0 - start.x) * deltaDist.x;
    sideDist.y = (dir.y < 0 ? start.y - cell.y : cell.y + 1.0 - start.y) * deltaDist.y;

    // ameddig tart a bor addig megyek
    // TODO: ha esetleg out of bounds lennénk
    while (space[cell.x][cell.y] == 0)
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

Ray::Ray(const Ray &ray) : start(ray.start),
                           end(ray.end),
                           dir(ray.dir),
                           sideDist(ray.sideDist),
                           wallDist(ray.wallDist),
                           side(ray.side),
                           space(ray.space) {}

double Ray::WallX()
{
    double wallX;
    if (side == 0)
        wallX = start.y + wallDist * dir.y; // posY + perpWallDist * rayDirY;
    else
        wallX = start.x + wallDist * dir.x;
    wallX -= floor(wallX);
    return wallX;
}

int Ray::CellValue()
{
    return space[cell.x][cell.y];
}
