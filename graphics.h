#if !defined(GRAPHICS_H)
#define GRAPHICS_H

#include "iostream"
#include "string"

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
    const int width, height; /**< ablak szélessége és magassága pixelben */
    SDL_Window *window;      /**< SDL ablak pointere */
    SDL_Renderer *renderer;  /**< SDL renderer pointere */
    Uint32 format;           /**< színformátum */

    Texture frameBuffer;             /**< a következő képkocka puffere */
    double *zBuffer;                 /**< vízszintes pixelenként tárolt mélység */
    Uint32 floorColor, cielingColor; /**< padló és plafon színe */
    Palette wallTextures;            /**< falak textúrái */
    Palette spriteTextures;          /**< entitások textúrái */

public:
    /**
     * konstuktor
     * @param w ablak szélessége
     * @param h ablak magassága
     * @param textures textúrák nevét tartalmazó fájl neve
     */
    Window(int w, int h, const char *textures);

    /**
     * destruktor
     */
    ~Window();

    /**
     * ablak törlése és előkészítése a következő képkockához
     */
    void Clear();

    /**
     * renderer tartalmának megjelenítése az ablakon
     */
    void Render();

    /**
     * szem elé vetített kijelző rendererre rajzolása (fps, hp)
     * @param game játékállás
     * @param fps képkockasebesség
     */
    void DrawHUD(const Game &game, int fps);

    /**
     * perspektíva (falak) kirajzolása
     * @param game játékállás
     */
    void DrawPerspective(const Game &game);

    /**
     * entitások kirajzolása
     * @param game játékállás
     */
    void DrawSprites(const Game &game);
};

class WindowInput : public Input
{
private:
    bool w = 0, a = 0, s = 0, d = 0; /**< billentyűk lenyomottsági állapota */

public:
    /**
     * alapértelmezett konstruktor
    */
    WindowInput();

    /**
     * billentyűzet állapotának lekérdezése
     * @param keyEvent SDL billentyűzet event
    */
    void UpdateKeys(const SDL_KeyboardEvent &keyEvent);

    /**
     * egér állapotának lekérdezése
     * @param mouseEvent SDL egér event
    */
    void UpdateMouse(const SDL_MouseMotionEvent &mouseEvent);

    /**
     * lövés kiváltó settere
    */
    void SetShootTrigger();
};

#endif // GRAPHICS_H
