#if !defined(GRAPHICS_H)
#define GRAPHICS_H


#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

class Window
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    Window(/* args */);
    ~Window();
};



#endif // GRAPHICS_H




