#include <iostream>
#include <math.h>
#define SDL_MAIN_HANDLED 
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

class vector2
{
public:
    double x, y;
    vector2(double x, double y);
};

vector2::vector2(double x, double y)
{
    this->x = x;
    this->y = y;
}



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

    vector2 playerPos = vector2(2, 3);
    double rotation = 0;
    

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Event mainEvent;

    while (true)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        
        // render level
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (levelGrid[i][j])
                {
                    SDL_Rect box = {};
                    rectangleColor(renderer, i*25, j*25, (i+1)*25, (j+1)*25, 0xFFFFFFFF);
                }
            }
            
        }
        circleColor(renderer, playerPos.x*25, playerPos.y*25, 5, 0xFF0000FF);
        
        for (int i = 0; i < 800; i++)
        {
            //raycast
            // 1 tile = 1 unit, might increase to 16 later

            
            vector2 ray = playerPos;

            double distance = 0;
            double precision = .01;
            for (distance = 0; distance < 10; distance += precision)
            {
                double radAngle = (double(i)-400)/400 * (M_PI/4) + rotation;
                ray.x += sin(radAngle)*precision; //todo
                ray.y += cos(radAngle)*precision;
                //cellcheck
                int cellx = ray.x;
                int celly = ray.y;
                if (levelGrid[cellx][celly])
                {            
                    //abs(distance *= cos(radAngle));
                    break;
                }
                
                
            }

            lineColor(renderer, playerPos.x*25, playerPos.y*25, ray.x*25, ray.y*25, 0x00FF00FF);
            SDL_RenderDrawLine(renderer, i, 300-1/distance*100, i, 300+1/distance*100);

            
        }

        SDL_RenderPresent(renderer);

        rotation += .01;
        
        
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

    std::cout << "Hello" << std::endl;
    return EXIT_SUCCESS;
}
