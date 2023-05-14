#include "graphics.h"

Window::Window(int w, int h) : width(w), height(h), format(SDL_PIXELFORMAT_ABGR8888), wallTextures(), frameBuffer()
{
    // initialize sdl
    SDL_Init(SDL_INIT_EVERYTHING);

    // create window and renderer
    // vices érted mert telefonkönyv oszt 3d xddddddd
    window = SDL_CreateWindow("Telefonkönyv", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_SHOWN);
    //SDL_RenderSetLogicalSize(renderer, width, height);

    // set default renderer of textures
    Texture::windowRenderer = renderer;

    // set color format
    Texture::windowFormat = format;
    std::cout << SDL_GetPixelFormatName(format) << std::endl;

    // load textures
    wallTextures.AddTexture("./ass/placeholder.png");
    wallTextures.AddTexture("./ass/sourcewall_og.png");
    wallTextures.AddTexture("./ass/cobblestone_mossy.png");
    wallTextures.AddTexture("./ass/cobblestone.png");

    spriteTextures.AddTexture("./ass/placeholder.png");
    spriteTextures.AddTexture("./ass/fokyouman.png");
    spriteTextures.AddTexture("./ass/glas.png");
    spriteTextures.AddTexture("./ass/citen.png");

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
    delete[] zBuffer;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::Clear()
{
    // empty the framebuffer
    frameBuffer.Lock();
    frameBuffer.ClearScreen();
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
    // put the contents of the renderer to the screen
    SDL_RenderPresent(renderer);
}

void Window::DrawMinimap(const Game &game)
{
    // draw tiles
    for (int i = 0; i < game.GetLevel().GetSize().x; i++)
        for (int j = 0; j < game.GetLevel().GetSize().y; j++)
            if (game.GetLevel()[i][j] != 0)
                rectangleColor(renderer, i * 10, j * 10, (i + 1) * 10, (j + 1) * 10, 0xFFFFFFFF);

    // draw entities
    for (auto i = game.GetEntities().begin(); i != game.GetEntities().end(); ++i)
        circleColor(renderer, i->GetPos().x * 10, i->GetPos().y * 10, 2, 0x00FF00FF);

    // draw player
    circleColor(renderer, game.GetPlayer().GetPos().x * 10, game.GetPlayer().GetPos().y * 10, 2, 0xFF0000FF);
}

void Window::DrawPerspective(const Game &game)
{
    // aliases
    const Player& player = game.GetPlayer();

    // lock buffer so we can write it
    frameBuffer.Lock();

    // irányra merőleges vektor, azaz a kamera síkjával párhuzamos
    Vector2 plane = player.GetPlane();
    for (int x = 0; x < width; x++)
    {
        // jelenlegi képernyőoszlop kamerához relatív aránya -1...1
        double camX = 2 * double(x) / width - 1;

        // sugár irányvektora
        Vector2 rayDir = player.GetDir() + plane * camX;

        // a sugár
        Ray cast = Ray(game.GetLevel(), player.GetPos(), rayDir);

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
                // pixel = (pixel >> 1) & 0xFF7F7F7F;
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

    // copy the framebuffer onto the screen
    SDL_RenderCopy(renderer, frameBuffer.texture, nullptr, nullptr);

}

void Window::DrawSprites(const Game &game)
{
    // aliases
    const Player& player = game.GetPlayer();
    
    // lezárjuk, mert rajzolni fogunk
    frameBuffer.Lock();

    // entitások szortírozó tömbje
    // tárulunk egy entitrásra mutató pointert, és a hozzá tartozó távolságot a játékostól
    int entSize = game.GetEntities().Size();
    Pair<const Entity *, double> *sortedEnts = new Pair<const Entity *, double>[entSize];

    // távolságok kiszámítása
    for (int i = 0; i < entSize; i++)
    {
        sortedEnts[i].a = &game.GetEntities()[i];
        sortedEnts[i].b = (player.GetPos() - sortedEnts[i].a->GetPos()).abs();
    }

    // buborékrendezés, legtávolabbi legelől a tömbben
    for (int i = 0; i < entSize - 1; i++)
        for (int j = 0; j < entSize - i - 1; j++)
            if (sortedEnts[j].b < sortedEnts[j + 1].b)
            {
                Pair<const Entity *, double> temp = sortedEnts[j];
                sortedEnts[j] = sortedEnts[j + 1];
                sortedEnts[j + 1] = temp;
            }

    // megjelenítés a kamerán
    for (int i = 0; i < entSize; i++)
    {
        // alias
        const Entity &ent = *(sortedEnts[i].a);
        const Vector2 dir = player.GetDir();
        const Vector2 plane = player.GetPlane();
        const Texture &spriteTex = spriteTextures[ent.GetID()];

        // játékoshoz relatív pozíciója
        Vector2 entPosPlayerSpace = ent.GetPos() - player.GetPos();

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

        // nem éri meg foglalkozni vele mert nem látjuk
        if (entPosCameraSpace.y < 0)
            continue;

        // a sprite közepének képernyőn vett koordinátája
        int spriteScreenX = (width / 2) * (1 + entPosCameraSpace.x / entPosCameraSpace.y);

        // sprite mérete a képernyőn
        // nincs külön szélesség és magasság, minden sprite négyzet alakú, a képek nyújtva lesznek
        int spriteSize = abs(int(height / (entPosCameraSpace.y)));

        Duo<int> drawStart, drawEnd;
        // sprite alja és teteje a képernyőn
        drawStart.y = -spriteSize / 2 + height / 2;
        if (drawStart.y < 0)
            drawStart.y = 0;
        drawEnd.y = spriteSize / 2 + height / 2;
        if (drawEnd.y >= height)
            drawEnd.y = height - 1;

        // sprite szélei a képernyőn
        drawStart.x = -spriteSize / 2 + spriteScreenX;
        if (drawStart.x < 0)
            drawStart.x = 0;
        drawEnd.x = spriteSize / 2 + spriteScreenX;
        if (drawEnd.x >= width)
            drawEnd.x = width - 1;

        if (drawEnd.x < drawStart.x)
            continue;
        


        // a sprite minden oszlopán végigmegyünk
        int firstStripe = drawStart.x;
        for (; firstStripe < drawEnd.x; firstStripe++)
            // első rajzolandó pixel
            if (entPosCameraSpace.y < zBuffer[firstStripe])
                break;

        int lastStripe = drawEnd.x;
        for (; lastStripe > drawStart.x; lastStripe--)
            // első rajzolandó pixel
            if (entPosCameraSpace.y > 0 && entPosCameraSpace.y < zBuffer[lastStripe])
                break;

        // optimization
        if (lastStripe == drawStart.x && firstStripe == drawEnd.x)
            continue;


        SDL_Rect onScreenDestination = {firstStripe, drawStart.y, lastStripe - firstStripe, drawEnd.y - drawStart.y};
        SDL_Rect onTextureSource = {0, 0, 0, 0};
        onTextureSource.x = (firstStripe - (-spriteSize / 2 + spriteScreenX)) * spriteTex.width / spriteSize;
        onTextureSource.w = (lastStripe - (-spriteSize / 2 + spriteScreenX)) * spriteTex.width / spriteSize - onTextureSource.x;
        onTextureSource.y = ((drawStart.y - height / 2 + spriteSize / 2) * spriteTex.height) / spriteSize;
        onTextureSource.h = ((drawEnd.y - height / 2 + spriteSize / 2) * spriteTex.height) / spriteSize - onTextureSource.y;

        SDL_RenderCopy(renderer, spriteTex.texture, &onTextureSource, &onScreenDestination);

        // known bug: placeholder flashes (suspicion: too small texture)
    }

    delete[] sortedEnts;
    frameBuffer.UnLock();
}

WindowInput::WindowInput() : Input() {}

void WindowInput::UpdateKeys(const SDL_KeyboardEvent& keyEvent)
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

void WindowInput::UpdateMouse(const SDL_MouseMotionEvent& mouseEvent)
{
    const double sensitivity = 0.5;
    turn += mouseEvent.xrel * sensitivity;
}

void WindowInput::SetShootTrigger()
{
    shootTrigger = true;
}