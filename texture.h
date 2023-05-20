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
    static SDL_Renderer *windowRenderer;
    static Uint32 windowFormat;
    SDL_Texture *texture;
    Duo<int> size;
    Uint32 *pixels;
    int pitch;

public:
    Texture();
    Texture(const char *fileName);
    Texture(const int w, const int h);
    Texture(const Texture &);

    Texture &operator=(const Texture &);

    ~Texture();

    static void SetRenderer(SDL_Renderer *renderer);
    static void SetFormat(Uint32 format);

    static Uint32 AlphaBlend(Uint32 base, Uint32 add);

    Uint32 GetPixel(int x, int y) const;
    void SetPixel(int x, int y, Uint32 set);
    SDL_Texture *GetTexture();
    const SDL_Texture *GetTexture() const;
    const Duo<int> &GetSize() const;

    void Clear();
    void ClearScreen();

    void Lock();
    void UnLock();
};

class Palette
{
private:
    Texture *placeholder;
    int size;
    Texture **textures;

public:
    Palette();
    ~Palette();

    void GeneratePlaceholder();
    void AddTexture(const char *, bool darken = false);
    void AddTexture(Texture *t, bool darken = false);

    // sdl limitációk miatt nem lehet konstans textúrával visszatérni
    Texture &operator[](int index);
    const Texture &operator[](int index) const;
};

#endif // TEXTURE_H
