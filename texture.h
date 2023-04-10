#if !defined(TEXTURE_H)
#define TEXTURE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"


class Color
{
public:
    Uint8 r, g, b, a;

    Color();
    Color(const Color& c);

    // pixelből szín
    Color(Uint32);
    // színből pixel
    Uint32 ToPixel();

    Color& Exposure(double e);

    //~Color();
};


class Texture
{
private:
    SDL_Surface *texture;
    
public:
    int width, height;

    Texture(const char* fileName);
    Texture(const Texture& t);
    ~Texture();

    Uint32 GetPixel(int x, int y);
    Color GetColor(int x, int y);
};






#endif // TEXTURE_H
