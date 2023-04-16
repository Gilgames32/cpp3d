#include "texture.h"

Uint32 Texture::windowFormat = SDL_PIXELFORMAT_ABGR8888;
SDL_Renderer *windowRenderer = nullptr;

SDL_Renderer* Texture::windowRenderer = nullptr;

Texture::Texture() : texture(nullptr), width(0), height(0), format(0), pixels(nullptr), pitch(0) {}

Texture::Texture(const char *fileName) : pixels(nullptr), pitch(0)
{
    SDL_Surface *image = IMG_Load(fileName);
    if (image == nullptr)
    {
        throw "WPO0HL";
    }
    width = image->w;
    height = image->h;
    format = windowFormat;
    texture = SDL_CreateTexture(windowRenderer, format, SDL_TEXTUREACCESS_STREAMING, width, height);
    Lock();
    SDL_ConvertPixels(width, height, image->format->format, image->pixels, image->pitch, format, pixels, pitch);
    UnLock();
}

Texture::Texture(const int w, const int h, Uint32 textureFormat) : width(w), height(h), format(textureFormat), pixels(nullptr), pitch(0){
    texture = SDL_CreateTexture(windowRenderer, textureFormat, SDL_TEXTUREACCESS_STREAMING, w, h);
}

Texture::Texture(const Texture& t) : width(t.width), height(t.height), format(t.format) {
    texture = SDL_CreateTexture(windowRenderer, format, SDL_TEXTUREACCESS_STREAMING, width, height);
    Lock();
    SDL_ConvertPixels(width, height, t.format, t.pixels, t.pitch, format, pixels, pitch);
    UnLock();
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
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

void Texture::Clear(){
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pixels[y * (pitch / sizeof(Uint32)) + x] = 0;
        }
    }
}