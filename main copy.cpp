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
    Input inp;
    Level lev("palya.txt");
    Player pla(Vector2(2, 3));


    SDL_Event mainEvent;
    bool quit = false;
    while (!quit)
    {
        win.DrawPerspective(lev, pla);
        win.Render();
        std::cout << "drawn" << std::endl;
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
                    win.DrawMinimap(lev);
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
