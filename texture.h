#if !defined(TEXTURE_H)
#define TEXTURE_H

#include "iostream"

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"

#include "vector2.h"
#include "gamelogic.h"

#include "memtrace.h"

class Texture
{
private:
    static SDL_Renderer *windowRenderer; /**< ablakhoz tartozó renderer */
    static Uint32 windowFormat;          /**< ablak színformátuma */
    SDL_Texture *texture;                /**< SDL textúra pointer */
    Duo<size_t> size;                    /**< méret */
    Uint32 *pixels;                      /**< SDL textúra pixeleinek címe */
    int pitch;                           /**< SDL textúra pixeleinek sorhossza */

public:
    /**
     * konstruktor
     */
    Texture();

    /**
     * kép fájl alapján textúrát betöltő konstruktor
     * @param fileName kép fájlneve
     * @throw hibás fájl(név)
     */
    Texture(const char *fileName);

    /**
     * méretek alapján üres textúrát készítő konstruktor
     * @param w szélesség
     * @param h magasság
     */
    Texture(const int w, const int h);

    /**
     * másoló konstruktor
     */
    Texture(const Texture &);

    /**
     * értékadó operátor
     */
    Texture &operator=(const Texture &);

    /**
     * destruktor
     */
    ~Texture();

    /**
     * ablakhoz tartozó renderer settere
     */
    static void SetRenderer(SDL_Renderer *renderer);

    /**
     * ablak színformátumának settere
     */
    static void SetFormat(Uint32 format);

    /**
     * áttetszőségi csatorna alapján összemos 2 színt
     * ABGR és ARGB formátumban működik
     * @param base alap/alsó szín
     * @param add hozzáadott/felső szín
     * @return kevert szín
     */
    static Uint32 AlphaBlend(Uint32 base, Uint32 add);

    /**
     * adott pixel színének gettere
     * @param x pixel x koordinátája
     * @param y pixel y koordinátája
     * @return adott pixel színe
     */
    Uint32 GetPixel(size_t x, size_t y) const;

    /**
     * adott pixel színének settere
     * @param x pixel x koordinátája
     * @param y pixel y koordinátája
     * @throw textúrán kívül eső pixel
     */
    void SetPixel(size_t x, size_t y, Uint32 set);

    /**
     * textúrához tartozó SDL textúra gettere
     * @return SDL textúra
     */
    SDL_Texture *GetTexture();

    /**
     * textúrához tartozó SDL textúra konstans gettere
     * @return SDL textúra
     */
    const SDL_Texture *GetTexture() const;

    /**
     * textúra méretének gettere
     * @return méret
     */
    const Duo<size_t> &GetSize() const;

    /**
     * textúra letörlése
     */
    void Clear();

    /**
     * textúra feltöltése a padló és a plafon színeivel
     * @param floor padló színe
     * @param cieling plafon színe
     */
    void ClearScreen(Uint32 floor, Uint32 cieling);

    /**
     * textúra lezárása
     */
    void Lock();

    /**
     * textúra feloldása
     */
    void UnLock();
};

class Palette
{
private:
    Texture *placeholder; /**< alapértelmezett helyörző textúra pointere */
    size_t size;          /**< paletta mérete */
    Texture **textures;   /**< textúrák pointereinek tömbje */

public:
    /**
     * konstruktor
     */
    Palette();

    /**
     * destruktor
     */
    ~Palette();

    /**
     * alapértelmezett helyörző textúra generálása
     * előkészít egy 2x2-es magenta-fekete saktáblamintás textúrát
     */
    void GeneratePlaceholder();

    /**
     * fájl alapján textúra hozzáadása a palettához
     * @param s fájlnév
     * @param darken true esetén egy sötétített verziót is hozzáad a palettához
     */
    void AddTexture(const char *s, bool darken = false);

    /**
     * fájl alapján textúra hozzáadása a palettához
     * @param s fájlnév
     * @param darken true esetén egy sötétített verziót is hozzáad a palettához
     */
    void AddTexture(Texture *t, bool darken = false);

    /**
     * indexelő operátor, SDL limitációk miatt nem lehet konstans
     * @param index index
     * @return indexedik textúra
     */
    Texture &operator[](size_t index);
};

#endif // TEXTURE_H
