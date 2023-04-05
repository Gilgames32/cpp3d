#if !defined(GRAPHICS_H)
#define GRAPHICS_H

#include "iostream"
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2.h"
#include "gamelogic.h"


class Window
{
private:
    int width, heigth;
    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    Window(int = 800, int = 600);
    ~Window();
    void Clear();
    void Render();
    void DrawMinimap(const Game&);
    void DrawPerspective(const Game&);

};


class WindowInput : private Input
{
public:
    WindowInput();
    //~Input();
    void UpdateKeys(SDL_KeyboardEvent keyEvent);
    void UpdateMouse(SDL_MouseMotionEvent mouseEvent);
};


#endif // GRAPHICS_H




