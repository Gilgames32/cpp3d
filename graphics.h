#if !defined(GRAPHICS_H)
#define GRAPHICS_H

#include "iostream"

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2.h"
#include "gamelogic.h"
#include "raycast.h"
#include "texture.h"

#include "memtrace.h"


class Window
{
private:
    const int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 format;

    Texture frameBuffer;
    double *zBuffer;
    Palette wallTextures;
    Palette spriteTextures;

public:
    Window(int w = 800, int h = 600);
    ~Window();
    void Clear();
    void Render();
    void DrawMinimap(const Game& game);
    void DrawPerspective(const Game& game);
    void DrawSprites(const Game& game);
};

class WindowInput : public Input
{
private:
    bool w = 0, a = 0, s = 0, d = 0;

public:
    WindowInput();
    //~Input();
    void UpdateKeys(const SDL_KeyboardEvent& keyEvent);
    void UpdateMouse(const SDL_MouseMotionEvent& mouseEvent);
    void SetShootTrigger();
};

#endif // GRAPHICS_H
