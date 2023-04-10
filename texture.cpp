#include "texture.h"

Color::Color() : r(0), g(0), b(0), a(0) {}

Color::Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

Color::Color(Uint32 pixel)
{
    r = pixel & 0xff;
    g = (pixel >> 8) & 0xff;
    b = (pixel >> 16) & 0xff;
    a = (pixel >> 24) & 0xff;
}

Uint32 Color::ToPixel()
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

Color& Color::Exposure(double e){
    r *= e;
    g *= e;
    b *= e;
    return *this;
}

Texture::Texture(const char *fileName)
{
    texture = IMG_Load(fileName);
    if (texture == nullptr)
    {
        throw "WPO0HL";
    }
    width = texture->w;
    height = texture->h;
}

Texture::Texture(const Texture &t) : width(t.width), height(t.height)
{
    // nem másolódik csak a pointere;
    texture = t.texture;
}

Texture::~Texture()
{
    // SDL_FreeSurface(texture);
}

Uint32 Texture::GetPixel(int x, int y)
{
    // bitmélység
    const Uint8 bSize = texture->format->BytesPerPixel;
    // addot pixel színe
    Uint8 *pixel = (Uint8 *)texture->pixels + y * texture->pitch + x * bSize;
    Uint32 pixelColor = *(Uint32 *)pixel;
    Color c;

    // this is a terrible way of doing this
    SDL_GetRGBA(pixelColor, texture->format, &c.r, &c.g, &c.b, &c.a);

    return c.ToPixel();
}

Color Texture::GetColor(int x, int y)
{
    return Color(GetPixel(x, y));
}
