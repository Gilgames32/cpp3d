#include "graphics.h"

Window::Window(int w, int h) : width(w), height(h), format(SDL_PIXELFORMAT_ABGR8888), wallTextures(), frameBuffer()
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
    std::cout << SDL_GetPixelFormatName(format) << std::endl;

    // load textures
    wallTextures.AddTexture("./ass/placeholder.png");
    wallTextures.AddTexture("./ass/grass_side.png");
    wallTextures.AddTexture("./ass/rolopipi.png");

    spriteTextures.AddTexture("./ass/placeholder.png");
    spriteTextures.AddTexture("./ass/fokyouman.png");

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
    // copy the framebuffer onto the screen
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

    // draw entities
    for (int i = 0; i < game.entSize; i++)
        circleColor(renderer, game.entities[i].pos.x * 10, game.entities[i].pos.y * 10, 2, 0x00FF00FF);

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
        int lineHeight = height / cast.wallDist;
        // legfelső rajzolható pixel
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0)
            drawStart = 0;
        // legalsó rajzolható pixel
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height)
            drawEnd = height - 1;

        // adott textúra
        Texture &pattern = wallTextures[cast.CellValue()];

        // textúra X oszlopa
        int textureX = cast.WallX() * double(pattern.width);

        // bizonyos oldalak tükrözve jelennek meg, erre fix
        if (cast.side == 0 && cast.dir.x < 0 || cast.side == 1 && cast.dir.y > 0)
            textureX = pattern.width - textureX - 1;

        // nyújtás mértéke
        double scale = double(pattern.height) / lineHeight;

        // ha nem látszódik az egész textúra, a tetejét elhagyjuk
        double textureY = (drawStart - height / 2 + lineHeight / 2) * scale;

        // méretre nyújtjuk/zsugorítjuk a textúrát
        for (int y = drawStart; y < drawEnd; y++)
        {
            // textúra adott pixele
            Uint32 pixel = pattern.GetPixel(textureX, textureY);
            textureY += scale;

            // igény szerint sötétítés
            if (cast.side)
                //pixel = (pixel >> 1) & 0xFF7F7F7F;
                pixel = Texture::AlphaBlend(0xFF000000, pixel & 0x77FFFFFF);

            // bufferbe írjuk
            frameBuffer.SetPixel(x, y, pixel);
        }

        // load it to the zbuffer
        zBuffer[x] = cast.wallDist;

        // minimap for debug
        lineColor(renderer, cast.end.x * 10, cast.end.y * 10, cast.start.x * 10, cast.start.y * 10, 0x0000FFFF);
    }

    // unluck framebuffer
    frameBuffer.UnLock();
}

void Window::DrawSprites(const Game &game)
{
    // lezárjuk, mert rajzolni fogunk
    frameBuffer.Lock();
    
    // entitások szortírozó tömbje
    // tárulunk egy entitrásra mutató pointert, és a hozzá tartozó távolságot a játékostól
    pair<Entity *, double> *sortedEnts = new pair<Entity *, double>[game.entSize];

    // távolságok kiszámítása
    for (int i = 0; i < game.entSize; i++)
    {
        sortedEnts[i].a = game.entities + i;
        Vector2 distance = game.player.pos - sortedEnts[i].a->pos;
        sortedEnts[i].b = distance.abs();
    }

    // buborékrendezés, legtávolabbi legelől a tömbben
    for (int i = 0; i < game.entSize - 1; i++)
        for (int j = 0; j < game.entSize - i - 1; j++)
            if (sortedEnts[j].b < sortedEnts[j + 1].b)
            {
                pair<Entity *, double> temp = sortedEnts[j];
                sortedEnts[j] = sortedEnts[j + 1];
                sortedEnts[j + 1] = temp;
            }

    // megjelenítés a kamerán
    for (int i = 0; i < game.entSize; i++)
    {
        // alias
        Entity &ent = *(sortedEnts[i].a);
        Vector2 dir = game.player.dir;
        Vector2 plane = game.player.plane();
        Texture &spriteTex = spriteTextures[ent.id];

        // játékoshoz relatív pozíciója
        Vector2 entPosPlayerSpace = ent.pos - game.player.pos;

        // kamera mátrixának inverzével transzformáljuk az entitás pozícióját
        // így megkapjuk a kamerához relatív pozícióját
        // BSZ1 my beloved
        /*
            [ planeX   dirX ] ^-1                 1              [ dirY      -dirX ]
            [               ]       =  -----------------------   [                 ]
            [ planeY   dirY ]          planeX*dirY-dirX*planeY   [ -planeY  planeX ]
        */

        // mátrix determiniánsának reciproka
        double detRec = 1.0 / (plane.x * dir.y - dir.x * plane.y);
        Vector2 entPosCameraSpace;
        entPosCameraSpace.x = detRec * (dir.y * entPosPlayerSpace.x - dir.x * entPosPlayerSpace.y);
        // mivel a kamera síkjára merőleges ezért igazából y egy kamerától való mélységet jelent
        entPosCameraSpace.y = detRec * (-plane.y * entPosPlayerSpace.x + plane.x * entPosPlayerSpace.y);

        // a sprite közepének képernyőn vett koordinátája
        int spriteScreenX = (width / 2) * (1 + entPosCameraSpace.x / entPosCameraSpace.y);

        // calculate height of the sprite on screen
        int spriteHeight = abs(int(height / (entPosCameraSpace.y))); // using "transformY" instead of the real distance prevents fisheye
        // calculate lowest and highest pixel to fill in current stripe
        int drawStartY = -spriteHeight / 2 + height / 2;
        if (drawStartY < 0)
            drawStartY = 0;
        int drawEndY = spriteHeight / 2 + height / 2;
        if (drawEndY >= height)
            drawEndY = height - 1;

        // calculate width of the sprite
        int spriteWidth = abs(int(height / (entPosCameraSpace.y))); // same as height of sprite, given that it's square
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX > width)
            drawEndX = width;

        // loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            // the conditions in the if are:
            // 1) it's in front of camera plane so you don't see things behind you
            // 2) ZBuffer, with perpendicular distance
            if (entPosCameraSpace.y > 0 && entPosCameraSpace.y < zBuffer[stripe])
            {
                int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * spriteTex.width / spriteWidth) / 256;
                for (int y = drawStartY; y < drawEndY; y++) // for every pixel of the current stripe
                {
                    int d = y * 256 - height * 128 + spriteHeight * 128; // 256 and 128 factors to avoid floats
                    int texY = ((d * spriteTex.height) / spriteHeight) / 256;
                    Uint32 color = spriteTex.GetPixel(texX, texY); // get current color from the texture
                    if ((color & 0xFF000000) != 0)
                        frameBuffer.SetPixel(stripe, y, Texture::AlphaBlend(frameBuffer.GetPixel(stripe, y), color)); // paint pixel if it isn't black, black is the invisible color
                        
                }
            }
        }
    }

    delete[] sortedEnts;
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