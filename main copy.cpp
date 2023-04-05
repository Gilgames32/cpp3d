#include <iostream>
#include <math.h>
#define SDL_MAIN_HANDLED 

#include "graphics.h"
#include "gamelogic.h"

// todo: put em in classes bit by bit
// trust me :clueless:

int main(int argc, char const *argv[])
{
    Window win;
    Game mainGame;
    WindowInput inp;


    SDL_Event mainEvent;
    bool quit = false;
    Uint32 prev = SDL_GetTicks(), curr = SDL_GetTicks(), deltaTime = 0;
    while (!quit)
    {
        // deltatime kiszámítása
        curr = SDL_GetTicks();
        deltaTime = curr - prev;
        prev = curr;

        //mainGame.SimulateGame(inp, deltaTime);
        
        while (SDL_PollEvent(&mainEvent))
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
                    win.Clear();
                    win.DrawPerspective(mainGame);
                    win.DrawMinimap(mainGame);
                    win.Render();
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
