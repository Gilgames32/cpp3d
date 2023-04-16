#if !defined(TEXTURE_H)
#define TEXTURE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include <iostream>


class Texture
{
private:
    
public:
    static SDL_Renderer *windowRenderer;
    static Uint32 windowFormat;

    SDL_Texture *texture;
    int width, height;
    Uint32* pixels;
    int pitch;

    Texture();
    Texture(const char* fileName);
    Texture(const int w, const int h);
    Texture(const Texture&);

    Texture& operator=(const Texture&);

    ~Texture();

    Uint32 GetPixel(int x, int y);
    void SetPixel(int x, int y, Uint32 pixel);
    void Clear();

    void Lock();
    void UnLock();
};






#endif // TEXTURE_H
