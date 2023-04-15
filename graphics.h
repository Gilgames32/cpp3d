#if !defined(GRAPHICS_H)
#define GRAPHICS_H

#include "iostream"
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2.h"
#include "gamelogic.h"
#include "raycast.h"
#include "texture.h"


class Window
{
private:
    int width, heigth;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 format;

    // must be pointer otherwise would need to exist before the renderer
    Texture *frameBuffer;
    Texture *pattern;
public:
    Window(int = 800, int = 600);
    ~Window();
    void Clear();
    void Render();
    void DrawMinimap(const Game&);
    void DrawPerspective(const Game&);

};


class WindowInput : public Input
{
    bool w = 0, a = 0, s = 0, d = 0;
public:
    WindowInput();
    //~Input();
    void UpdateKeys(SDL_KeyboardEvent keyEvent);
    void UpdateMouse(SDL_MouseMotionEvent mouseEvent);
};


#endif // GRAPHICS_H




