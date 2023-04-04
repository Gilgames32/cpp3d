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
    Window(int w = 800, int h = 600);
    ~Window();
    void Clear();
    void Render();
    void DrawMinimap();
    void DrawPerspective();

};


class Input
{
    Vector2 dir;
    double turn;
    bool w = 0, a = 0, s = 0, d = 0;
public:
    Input(Vector2 dir = Vector2());
    //~Input();
    void UpdateKeys(SDL_KeyboardEvent keyEvent);
    void UpdateMouse(SDL_MouseMotionEvent mouseEvent);
};


#endif // GRAPHICS_H




