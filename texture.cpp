#include "texture.h"

Uint32 Texture::windowFormat = SDL_PIXELFORMAT_ABGR8888;
SDL_Renderer *windowRenderer = nullptr;

SDL_Renderer *Texture::windowRenderer = nullptr;

Texture::Texture() : texture(nullptr), width(0), height(0), pixels(nullptr), pitch(0) {}

Texture::Texture(const char *fileName) : pixels(nullptr), pitch(0)
{
    SDL_Surface *image = IMG_Load(fileName);
    if (image == nullptr)
    {
        throw "WPO0HL";
    }
    width = image->w;
    height = image->h;
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
    Lock();
    SDL_ConvertPixels(width, height, image->format->format, image->pixels, image->pitch, windowFormat, pixels, pitch);
    UnLock();
}

Texture::Texture(const int w, const int h) : width(w), height(h), pixels(nullptr), pitch(0)
{
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, w, h);
    // FONTOS!! Lock hívás minden konstrukciónál a pixel és pitch beállításhoz
    Lock();
    Clear();
    UnLock();
}

Texture &Texture::operator=(const Texture &t)
{
    width = t.width;
    height = t.height;
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
    Lock();
    SDL_ConvertPixels(width, height, windowFormat, t.pixels, t.pitch, windowFormat, pixels, pitch);
    UnLock();
    return *this;
}

Texture::Texture(const Texture &t) : width(t.width), height(t.height)
{
    std::cout << "ennek nem kéne meghívódni" << std::endl;
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
    Lock();
    SDL_ConvertPixels(width, height, windowFormat, t.pixels, t.pitch, windowFormat, pixels, pitch);
    UnLock();
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

Uint32 Texture::GetPixel(int x, int y)
{
    if (x >= width || x < 0 || y >= height || y < 0)
        return 0x00000000;

    Uint32 pixelPosition = y * (pitch / sizeof(Uint32)) + x;
    return pixels[pixelPosition];
}

void Texture::SetPixel(int x, int y, Uint32 set)
{
    Uint32 pixelPosition = y * (pitch / sizeof(Uint32)) + x;
    pixels[pixelPosition] = set;
}

Uint32 Texture::AlphaBlend(Uint32 base, Uint32 add)
{
    // feltételezzük hogy ABGR vagy ARGB
    // 0xAABBGGRR

    double addAlpha = double(add >> 24 & 0xFF) / 255;
    if (addAlpha == 1)
        return add;
    if (addAlpha == 0)
        return base;

    // https://en.wikipedia.org/wiki/Alpha_compositing
    double baseAlpha = base >> 24 && 0xFF / 255;
    double omegAlpha = addAlpha + baseAlpha * (1 - addAlpha);
    Uint32 color = 0;
    Uint8 red = (addAlpha * (add >> 0 & 0xFF) + baseAlpha * (1 - addAlpha) * (base >> 0 & 0xFF)) / omegAlpha;
    Uint8 green = (addAlpha * (add >> 8 & 0xFF) + baseAlpha * (1 - addAlpha) * (base >> 8 & 0xFF)) / omegAlpha;
    Uint8 blue = (addAlpha * (add >> 16 & 0xFF) + baseAlpha * (1 - addAlpha) * (base >> 16 & 0xFF)) / omegAlpha;
    Uint8 alpha = omegAlpha * 255;

    color = alpha << 24 | blue << 16 | green << 8 | red << 0;
    return color;
}

void Texture::Lock()
{
    SDL_LockTexture(texture, nullptr, (void **)&pixels, &pitch);
}
void Texture::UnLock()
{
    SDL_UnlockTexture(texture);
}

void Texture::Clear()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pixels[y * (pitch / sizeof(Uint32)) + x] = y > height/2 ? 0xFF555555 : 0xFF222222;
        }
    }
}

Palette::Palette() : size(0), textures(nullptr) {}
Palette::~Palette()
{
    for (int i = 0; i < size; i++)
    {
        delete textures[i];
    }
    delete[] textures;
}

void Palette::AddTexture(Texture *t)
{
    Texture **temp = new Texture *[size + 1];
    for (int i = 0; i < size; i++)
    {
        temp[i] = textures[i];
    }
    temp[size] = t;
    size++;
    delete[] textures;
    textures = temp;
}

void Palette::AddTexture(const char *s)
{
    Texture *t = new Texture(s);
    AddTexture(t);
}

Texture &Palette::operator[](int index) const
{
    // todo: built in placeholder
    if (index >= size)
        return *(textures[0]);
    else
        return *(textures[index]);
}
