#include "texture.h"

Uint32 Texture::windowFormat = SDL_PIXELFORMAT_ABGR8888;
SDL_Renderer *windowRenderer = nullptr;

SDL_Renderer* Texture::windowRenderer = nullptr;

Texture::Texture() : texture(nullptr), width(0), height(0), format(0), pixels(nullptr), pitch(0) {}

Texture::Texture(const char *fileName) : pixels(nullptr), pitch(0)
{
    texture = IMG_LoadTexture(windowRenderer, fileName);
    // do this better ok?
    if (texture == nullptr)
    {
        throw "WPO0HL";
    }
    SDL_QueryTexture(texture, &format, nullptr, &width, &height);
}

Texture::Texture(const Texture &t) : width(t.width), height(t.height)
{
    // SDL_BlitSurface !!!
    // nem másolódik csak a pointere;
    texture = t.texture;
}

Texture::Texture(const int w, const int h, Uint32 format){
    texture = SDL_CreateTexture(windowRenderer, format, SDL_TEXTUREACCESS_STATIC, w, h);
}

Texture::~Texture()
{
    // SDL_FreeSurface(texture);
}

Uint32 Texture::GetPixel(int x, int y)
{
    Uint32 pixelPosition = y * (pitch / sizeof(Uint32)) + x;
    return pixels[pixelPosition];
}

void Texture::SetPixel(int x, int y, Uint32 set)
{
    Uint32 pixelPosition = y * (pitch / sizeof(Uint32)) + x;
    pixels[pixelPosition] = set;
}

void Texture::Lock(){
    SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);
}
void Texture::UnLock(){
    SDL_UnlockTexture(texture);
}