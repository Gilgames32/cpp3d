#include "texture.h"

Color::Color() : r(0), g(0), b(0), a(0) {}

Color::Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

Color::Color(Uint32 pixel){
    r = pixel & 0xff;
    g = (pixel>>8) & 0xff;
    b = (pixel>>16) & 0xff;
    a = (pixel>>24) & 0xff;
}

Uint32 Color::ToPixel(){
    return (r<<24) | (g<<16)| (b<<8) | a;
}

Texture::Texture(const char* fileName){
    texture = IMG_Load(fileName);
    if (texture == nullptr)
    {
        throw "WPO0HL";
    }
    width = texture->w;
    height = texture->h;
}

Texture::Texture(const Texture& t) : width(t.width), height(t.height){
    // nem másolódik csak a pointere;
    texture = t.texture;
}

Texture::~Texture(){
    // SDL_FreeSurface(texture);
}

Uint32 Texture::GetPixel(int x, int y){
    /*
    // egyelőre csak 4bájt (RGBA)/pixel képpel működik
    if (texture->format->BytesPerPixel != 4)
        throw "WPO0HL";
    
    // kezdő memóriacím
    Uint32 *pixel = (Uint32 *) texture->pixels;
    //eltolás sor * sorhosszal
    pixel += y * texture->pitch;
    //eltolás oszloppal
    pixel += x;

    return *pixel;    
    
    
    */
    // Bytes per pixel
    const Uint8 Bpp = texture->format->BytesPerPixel;

    /*
    Retrieve the address to a specific pixel
    pSurface->pixels  = an array containing the SDL_Surface' pixels
    pSurface->pitch       = the length of a row of pixels (in bytes)
    X and Y               = the offset on where on the image to retrieve the pixel; (0, 0) is the upper left corner
    */
    Uint8* pPixel = (Uint8*)texture->pixels + y * texture->pitch + x * Bpp;

    Uint32 PixelData = *(Uint32*)pPixel;
    

    Color pixelColor;
    // Retrieve the RGB values of the specific pixel
    SDL_GetRGBA(PixelData, texture->format, &pixelColor.r, &pixelColor.g, &pixelColor.b, &pixelColor.a);

    return pixelColor.ToPixel();

}

Color Texture::GetColor(int x, int y){
    return Color(GetPixel(x, y));
}

