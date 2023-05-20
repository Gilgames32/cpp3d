#include "texture.h"

Uint32 Texture::windowFormat = SDL_PIXELFORMAT_ABGR8888;

SDL_Renderer *Texture::windowRenderer = nullptr;

void Texture::SetRenderer(SDL_Renderer *renderer)
{
    windowRenderer = renderer;
}

void Texture::SetFormat(Uint32 format)
{
    windowFormat = format;
}

Texture::Texture() : texture(nullptr), size(Duo<int>(0, 0)), pixels(nullptr), pitch(0) {}

Texture::Texture(const char *fileName) : pixels(nullptr), pitch(0)
{
    SDL_Surface *image = IMG_Load(fileName);
    if (image == nullptr)
        //throw "NEPTUNKOD"; ...
        throw std::runtime_error("Nem betölthető a kép");

    size.x = image->w;
    size.y = image->h;
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);
    Lock();
    SDL_ConvertPixels(size.x, size.y, image->format->format, image->pixels, image->pitch, windowFormat, pixels, pitch);
    UnLock();
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

Texture::Texture(int w, int h) : size(Duo<int>(w, h)), pixels(nullptr), pitch(0)
{
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, w, h);
    // FONTOS!! Lock hívás minden konstrukciónál a pixel és pitch beállításhoz
    // how do i know? ...
    Lock();
    Clear();
    UnLock();
}

Texture &Texture::operator=(const Texture &t)
{
    size = t.size;
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);
    Lock();
    SDL_ConvertPixels(size.x, size.y, windowFormat, t.pixels, t.pitch, windowFormat, pixels, pitch);
    UnLock();
    return *this;
}

Texture::Texture(const Texture &t) : size(t.size)
{
    // not gonna lie ennek marhára nem kéne meghívódni
    texture = SDL_CreateTexture(windowRenderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);
    Lock();
    SDL_ConvertPixels(size.x, size.y, windowFormat, t.pixels, t.pitch, windowFormat, pixels, pitch);
    UnLock();
    SDL_BlendMode blend;
    SDL_GetTextureBlendMode(t.texture, &blend);
    SDL_SetTextureBlendMode(texture, blend);
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

Uint32 Texture::GetPixel(int x, int y) const
{
    // out of range esetén áttetsző pixelt adunk
    if (x >= size.x || x < 0 || y >= size.y || y < 0)
        return 0x00000000;

    Uint32 pixelPosition = y * (pitch / sizeof(Uint32)) + x;
    return pixels[pixelPosition];
}

void Texture::SetPixel(int x, int y, Uint32 set)
{
    if (x >= size.x || x < 0 || y >= size.y || y < 0)
        throw std::out_of_range("Textúrán kívüli pixel");

    Uint32 pixelPosition = y * (pitch / sizeof(Uint32)) + x;
    pixels[pixelPosition] = set;
}

Uint32 Texture::AlphaBlend(Uint32 base, Uint32 add)
{
    // feltételezzük, hogy ABGR vagy ARGB
    // 0xAABBGGRR (grrrrrr...)

    // skippeljük amikor csak lehet mert ez processzoron megy xd
    Uint16 skip = add >> 24 & 0xFF;
    if (skip == 0xFF)
        return add;
    if (skip == 0x00)
        return base;
    
    double addAlpha = double(skip) / 255;

    // https://en.wikipedia.org/wiki/Alpha_compositing
    // lehetőleg mellőzzük mert szegény 1 core amin fut nem nagyon szereti ezt pixelenként futtatni
    // T-T
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

const SDL_Texture *Texture::GetTexture() const
{
    return texture;
}

SDL_Texture *Texture::GetTexture()
{
    return texture;
}

const Duo<int> &Texture::GetSize() const
{
    return size;
}

void Texture::Lock()
{
    SDL_LockTexture(texture, nullptr, (void **)&pixels, &pitch);
}
void Texture::UnLock()
{
    SDL_UnlockTexture(texture);
}

void Texture::ClearScreen(Uint32 floor, Uint32 cieling)
{
    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            // képernyő fele ilyen a másik olyan, tökéletes az illúzióhot
            // todo: maybe hogy lehessen ezt is állítani valahogy
            pixels[y * (pitch / sizeof(Uint32)) + x] = y > size.y / 2 ? floor : cieling;
        }
    }
}

void Texture::Clear()
{
    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            pixels[y * (pitch / sizeof(Uint32)) + x] = 0;
        }
    }
}

Palette::Palette() : size(0)
{
    textures = new Texture *[size];
}

Palette::~Palette()
{
    for (int i = 0; i < size; i++)
    {
        delete textures[i];
    }
    delete[] textures;
    delete placeholder;
}

void Palette::GeneratePlaceholder()
{
    placeholder = new Texture(2, 2);
    placeholder->SetPixel(0, 0, 0xFFFF00FF);
    placeholder->SetPixel(1, 1, 0xFFFF00FF);
    placeholder->SetPixel(0, 1, 0xFF000000);
    placeholder->SetPixel(1, 0, 0xFF000000);
}

void Palette::AddTexture(Texture *t, bool darken)
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

    // add the dark version after if needed
    if (darken)
    {
        Texture* darkt = new Texture(*t);
        Duo size = darkt->GetSize();
        for (int x = 0; x < size.x; x++)
        {
            for (int y = 0; y < size.y; y++)
            {
                // todo: switch order of blending (?)
                darkt->SetPixel(x, y, Texture::AlphaBlend(0xFF000000, darkt->GetPixel(x, y) & 0x77FFFFFF));
            }   
        }
        // :nyakkendő:
        AddTexture(darkt, false);
    }
}

void Palette::AddTexture(const char *s, bool darken)
{
    Texture *t = new Texture(s);
    AddTexture(t, darken);
}

Texture &Palette::operator[](int index)
{
    // todo: built in placeholder
    if (index < 0 || index >= size)
        return *placeholder;
    else
        return *(textures[index]);
}

const Texture &Palette::operator[](int index) const
{
    return operator[](index);
}
