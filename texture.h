#if !defined(TEXTURE_H)
#define TEXTURE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"


class Texture
{
private:
    
public:
    static SDL_Renderer *windowRenderer;
    static Uint32 windowFormat;

    SDL_Texture *texture;
    int width, height;
    Uint32 format;
    Uint32* pixels;
    int pitch;

    Texture();
    Texture(const char* fileName);
    Texture(const Texture& t);
    Texture(const int w, const int h, Uint32 format);
    ~Texture();

    Uint32 GetPixel(int x, int y);
    void SetPixel(int x, int y, Uint32 pixel);

    void Lock();
    void UnLock();
};






#endif // TEXTURE_H
