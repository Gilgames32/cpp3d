#include <iostream>
#include <math.h>
#define SDL_MAIN_HANDLED 
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2.h"

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("UWU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    //SDL_SetWindowGrab(window, SDL_TRUE);



    bool levelGrid [6][6] = {
        {1,1,1,1,1,1},
        {1,0,0,0,0,1},
        {1,0,0,0,0,1},
        {1,0,0,1,1,1},
        {1,0,0,0,0,1},
        {1,1,1,1,1,1}
    };






    vector2 pPos = vector2(2, 3);
    vector2 pDir = vector2(-1, 0);
    vector2 pPlane = rotate(pDir, -M_PI/2);

    SDL_Event mainEvent;
    while (true)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // render minimap
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (levelGrid[i][j])
                {
                    rectangleColor(renderer, i*25, j*25, (i+1)*25, (j+1)*25, 0xFFFFFFFF);
                }
            }
            
        }
        circleColor(renderer, pPos.x*25, pPos.y*25, 5, 0xFF0000FF);
        





        

        for (int i = 0; i < 800; i+=1)
        {
            pair<int> mapCell = pair<int>(pPos.x, pPos.y);
            
            
        }







        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&mainEvent))
        {
            if (mainEvent.type == SDL_QUIT)
            {
                break;
            }
            
        }
        
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
