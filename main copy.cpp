#include <iostream>
#include <math.h>
#define SDL_MAIN_HANDLED 

#include "graphics.h"
//#include "gamelogic.h"

// todo: put em in classes bit by bit
// trust me :clueless:

int main(int argc, char const *argv[])
{
    bool levelGrid [16][16] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };



    Window win;
    Input inp;

    // vector2 pPlane = rotate(pDir, -M_PI/2);

    SDL_Event mainEvent;
    bool quit = false;
    while (!quit)
    {
        /*
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                if (levelGrid[i][j])
                {
                    rectangleColor(renderer, i*10, j*10, (i+1)*10, (j+1)*10, 0xFFFFFFFF);
                }
            }
            
        }
        circleColor(renderer, pPos.x*10, pPos.y*10, 2, 0xFF0000FF);
        

        for (int i = 0; i < w; i+=1)
        {
            //kiindulási cella cella
            pair<int> rayCell = pair<int>(pPos.x, pPos.y);

            //jelenlegi csík kamerához relatív aránya -1...1
            double camX = 2*double(i)/w - 1;

            //sugár irányvektora
            Vector2 rayDir = pDir + pPlane * camX;

            //rácsvonalanként léptetett pont
            Vector2 sideDist;

            //egyik x vagy y oldalról a legközelebbi átellenes oldalig a távolság
            //1 helyett átfogó kéne
            Vector2 deltaDist = Vector2(abs(1/rayDir.x), abs(1/rayDir.y));

            //lépegeté irányának előjele, -1 vagy 1
            pair<int> stepDir;
            stepDir.x = rayDir.x < 0 ? -1 : 1;
            stepDir.y = rayDir.y < 0 ? -1 : 1;

            //kamera síkja és a fal közti távolság
            double wallDist;

            //vízszintes / függőleges oldalt talált el
            bool side;


            //legközelebbi falig távolság
            if(rayDir.x < 0)
                sideDist.x = (pPos.x - rayCell.x) * deltaDist.x;
            else
                sideDist.x = (rayCell.x + 1.0 - pPos.x) * deltaDist.x;

            if(rayDir.y < 0)
                sideDist.y = (pPos.y - rayCell.y) * deltaDist.y;
            else
                sideDist.y = (rayCell.y + 1.0 - pPos.y) * deltaDist.y;

            while (levelGrid[rayCell.x][rayCell.y] == 0)
            {
                if(sideDist.x < sideDist.y)
                {
                    sideDist.x += deltaDist.x;
                    rayCell.x += stepDir.x;
                    side = false;
                }
                else
                {
                    sideDist.y += deltaDist.y;
                    rayCell.y += stepDir.y;
                    side = true;
                }
            }

            if(side == 0) wallDist = (sideDist.x - deltaDist.x);
            else          wallDist = (sideDist.y - deltaDist.y);
            
            int lineHeight = (int)(h*.5 / wallDist);
            int drawStart = -lineHeight / 2 + h / 2;
            if(drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if(drawEnd >= h) drawEnd = h - 1;

            lineColor(renderer, i, drawStart, i, drawEnd, side ? 0x888888FF : 0xFFFFFFFF);
            Vector2 hit = pPos + rayDir * wallDist;
            lineColor(renderer, hit.x * 10, hit.y * 10, pPos.x * 10, pPos.y * 10, 0x0000FFFF);
        }




        pDir.rotate(.01);
        pPlane = rotate(pDir, -M_PI/2);




        SDL_RenderPresent(renderer);
        */

        if (SDL_PollEvent(&mainEvent))
        {
            switch (mainEvent.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                // filter repeating signals
                if (!mainEvent.key.repeat)
                    inp.UpdateKeys(mainEvent.key);

                break;
            
            case SDL_MOUSEMOTION:
                inp.UpdateMouse(mainEvent.motion);
            default:
                break;
            }
        }

    }



    return EXIT_SUCCESS;
}
