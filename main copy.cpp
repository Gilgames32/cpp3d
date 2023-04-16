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
    Uint64 prev = SDL_GetPerformanceCounter(), curr = SDL_GetPerformanceCounter();
    double deltaTime = 0;
    while (!quit)
    {
        prev = curr;
        curr = SDL_GetPerformanceCounter();

        deltaTime = double((curr - prev) * 1000 / double(SDL_GetPerformanceFrequency()));
        // FPS
        // std::cout << (1000 / deltaTime) << std::endl;

        mainGame.SimulateGame(inp, deltaTime);

        win.Clear();
        win.DrawPerspective(mainGame);
        win.DrawMinimap(mainGame);
        win.Render();

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
                break;

            case SDL_MOUSEMOTION:
                inp.UpdateMouse(mainEvent.motion);
                break;

            case SDL_MOUSEBUTTONDOWN:
                SDL_SetRelativeMouseMode(SDL_TRUE);
                break;

            default:
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
