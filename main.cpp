#include <iostream>
#include <math.h>
#define SDL_MAIN_HANDLED 
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2.h"
#include "graphics.h"


#define w 800
#define h 600

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("UWU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    //SDL_SetWindowGrab(window, SDL_TRUE);



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






    vector2 pPos = vector2(2, 3);
    vector2 pDir = vector2(-1, 0);
    vector2 pPlane = rotate(pDir, -M_PI/2);

    SDL_Event mainEvent;
    bool quit = false;
    while (!quit)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // render minimap
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
            vector2 rayDir = pDir + pPlane * camX;

            //rácsvonalanként léptetett pont
            vector2 sideDist;

            //egyik x vagy y oldalról a legközelebbi átellenes oldalig a távolság
            //1 helyett átfogó kéne
            vector2 deltaDist = vector2(abs(1/rayDir.x), abs(1/rayDir.y));

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
            vector2 hit = pPos + rayDir * wallDist;
            lineColor(renderer, hit.x * 10, hit.y * 10, pPos.x * 10, pPos.y * 10, 0x0000FFFF);
        }




        pDir.rotate(.01);
        pPlane = rotate(pDir, -M_PI/2);




        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&mainEvent))
        {
            switch (mainEvent.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            
            default:
                break;
            }

            
            
        }
        
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
