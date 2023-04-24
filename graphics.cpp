#include "graphics.h"

Window::Window(int w, int h) : width(w), heigth(h), format(SDL_PIXELFORMAT_ABGR8888), wallTextures(), frameBuffer()
{
    // initialize sdl
    SDL_Init(SDL_INIT_EVERYTHING);

    // create window and renderer
    // todo: game name
    window = SDL_CreateWindow("NHZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_SHOWN);

    // set default renderer of textures
    Texture::windowRenderer = renderer;

    // set color format
    Texture::windowFormat = format;

    // load textures
    wallTextures.AddTexture("./ass/placeholder.png");
    wallTextures.AddTexture("./ass/grass_side.png");
    wallTextures.AddTexture("./ass/rolopipi.png");






    // create a framebuffer
    frameBuffer = Texture(w, h);
    SDL_SetTextureBlendMode(frameBuffer.texture, SDL_BLENDMODE_BLEND);

    // create zbuffer
    zBuffer = new double[width];

    // trap mouse uwu
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::Clear()
{
    // empty the framebuffer
    frameBuffer.Lock();
    frameBuffer.Clear();
    frameBuffer.UnLock();

    // clear the SDL renderer
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Window::Render()
{
    // copy the buffer onto the screen
    SDL_RenderCopy(renderer, frameBuffer.texture, nullptr, nullptr);

    // put the contents of the renderer to the screen
    SDL_RenderPresent(renderer);
}

void Window::DrawMinimap(const Game &game)
{
    // draw tiles
    for (int i = 0; i < game.level.sizex; i++)
        for (int j = 0; j < game.level.sizey; j++)
            if (game.level[i][j] != 0)
                rectangleColor(renderer, i * 10, j * 10, (i + 1) * 10, (j + 1) * 10, 0xFFFFFFFF);

    // draw player
    circleColor(renderer, game.player.pos.x * 10, game.player.pos.y * 10, 2, 0xFF0000FF);
}

void Window::DrawPerspective(const Game &game)
{
    // lock buffer so we can write it
    frameBuffer.Lock();

    // irányra merőleges vektor, azaz a kamera síkjával párhuzamos
    Vector2 plane = game.player.plane();
    for (int x = 0; x < width; x++)
    {
        // jelenlegi képernyőoszlop kamerához relatív aránya -1...1
        double camX = 2 * double(x) / width - 1;

        // sugár irányvektora
        Vector2 rayDir = game.player.dir + plane * camX;

        // a sugár
        Ray cast = Ray(game.level, game.player.pos, rayDir);

        // a fal magassága:
        int lineHeight = heigth / cast.wallDist;
        // legfelső rajzolható pixel
        int drawStart = -lineHeight / 2 + heigth / 2;
        if (drawStart < 0)
            drawStart = 0;
        // legalsó rajzolható pixel
        int drawEnd = lineHeight / 2 + heigth / 2;
        if (drawEnd >= heigth)
            drawEnd = heigth - 1;

        // adott textúra
        Texture& pattern = wallTextures[cast.CellValue()];

        // textúra X oszlopa
        int textureX = cast.WallX() * double(pattern.width);

        // bizonyos oldalak tükrözve jelennek meg, erre fix
        if (cast.side == 0 && cast.dir.x < 0 || cast.side == 1 && cast.dir.y > 0)
            textureX = pattern.width - textureX - 1;

        // nyújtás mértéke
        double scale = double(pattern.height) / lineHeight;

        // ha nem látszódik az egész textúra, a tetejét elhagyjuk
        double textureY = (drawStart - heigth / 2 + lineHeight / 2) * scale;

        // méretre nyújtjuk/zsugorítjuk a textúrát
        for (int y = drawStart; y < drawEnd; y++)
        {
            // textúra adott pixele
            Uint32 pixel = pattern.GetPixel(textureX, textureY);
            textureY += scale;

            // igény szerint sötétítés
            if (cast.side)
                pixel = (pixel >> 1) & 0xFF7F7F7F;

            // bufferbe írjuk
            frameBuffer.SetPixel(x, y, pixel);
        }

        // minimap for debug
        lineColor(renderer, cast.end.x * 10, cast.end.y * 10, cast.start.x * 10, cast.start.y * 10, 0x0000FFFF);
    }

    // unluck framebuffer
    frameBuffer.UnLock();
}

WindowInput::WindowInput() : Input() {}

void WindowInput::UpdateKeys(SDL_KeyboardEvent keyEvent)
{
    switch (keyEvent.keysym.sym)
    {
    case SDLK_ESCAPE:
        // unlock mouse
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;

    case SDLK_w:
        w = keyEvent.state;
        break;
    case SDLK_a:
        a = keyEvent.state;
        break;
    case SDLK_s:
        s = keyEvent.state;
        break;
    case SDLK_d:
        d = keyEvent.state;
        break;

    default:
        break;
    }

    dir = Vector2(d - a, w - s);
}

void WindowInput::UpdateMouse(SDL_MouseMotionEvent mouseEvent)
{
    turn += mouseEvent.xrel; // multiply by sensitivity latur
}