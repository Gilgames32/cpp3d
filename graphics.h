#if !defined(GRAPHICS_H)
#define GRAPHICS_H


#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2.h"

class Window
{
private:
    int width, heigth;
    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    Window(int w, int h);
    ~Window();
    void Clear();
    void Render();
    void DrawPerspective();

};


class Input
{
    Vector2 dir;
public:
    Input(/* args */);
    ~Input();
};


#endif // GRAPHICS_H




