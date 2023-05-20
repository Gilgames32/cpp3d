#include "graphics.h"

Window::Window(int w, int h) : width(w), height(h), format(SDL_PIXELFORMAT_ABGR8888), wallTextures(), frameBuffer()
{
    // sdl inicializálás
    SDL_Init(SDL_INIT_EVERYTHING);

    // ablak és renderer
    // vices érted mert telefonkönyv oszt 3d xddddddd
    window = SDL_CreateWindow("Telefonkönyv3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_SHOWN);
    // SDL_RenderSetLogicalSize(renderer, width, height); // ha esetleg pixelesíteni szeretnénk még rajt

    // alapértelmezett renderer a textúrákhoz
    // elmagyarázhatnám, hogy miért kell de tldr: az sdl fos
    Texture::SetRenderer(renderer);

    // szín format beállítás, hogy egységes legyen mer az jó
    Texture::SetFormat(format);
    std::cout << SDL_GetPixelFormatName(format) << std::endl;

    // alapértelmezett placeholder textúra inicializálás
    wallTextures.GeneratePlaceholder();
    spriteTextures.GeneratePlaceholder();

    // textúrák betöltése
    wallTextures.AddTexture("./ass/sourcewall_og.png", true);
    wallTextures.AddTexture("./ass/cobblestone_mossy.png", true);
    wallTextures.AddTexture("./ass/cobblestone.png", true);

    spriteTextures.AddTexture("./ass/fokyouman.png");
    spriteTextures.AddTexture("./ass/glas.png");
    spriteTextures.AddTexture("./ass/citen.png");

    // falaknak framebuffer
    frameBuffer = Texture(w, h);
    SDL_SetTextureBlendMode(frameBuffer.GetTexture(), SDL_BLENDMODE_BLEND);

    // zbuffer a mélységnek sprite renderhez
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
    // képernyőtörlés, padló és plafon beállítás
    frameBuffer.Lock();
    frameBuffer.ClearScreen();
    frameBuffer.UnLock();

    // hiszed vagy sem, ha ezt nem csináljuk meg, kb 5 mp után az elkezd droppolni 3 alá az fps
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Window::Render()
{
    // renderer tartalmát a képernyőre
    SDL_RenderPresent(renderer);
}

void Window::DrawHUD(const Game &game, int fps)
{
    // crosshair
    boxColor(renderer, width/2 -1, height/2 -1, width/2 +1, height/2 +1, 0xFFFFFFFF);
    
    // igen így is lehet betűméretet állítani xd
    SDL_RenderSetLogicalSize(renderer, width/2, height/2);
    stringColor(renderer, 4, 4,  ("FPS - " + std::to_string(fps)).c_str(), 0xFFFFFFFF);
    stringColor(renderer, 4, 20, (" HP - " + std::to_string(game.GetPlayer().GetHP())).c_str(), 0xFFFFFFFF);
    SDL_RenderSetLogicalSize(renderer, width, height);
}

void Window::DrawPerspective(const Game &game)
{
    // aliases
    const Player &player = game.GetPlayer();

    // lezárás, hogy tudjuk módosítani
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
        int lineHeight = height / cast.GetWallDist();
        // legfelső rajzolható pixel
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0)
            drawStart = 0;
        // legalsó rajzolható pixel
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height)
            drawEnd = height - 1;

        // adott textúra
        const Texture &pattern = wallTextures[(cast.CellValue()-1)*2 + cast.GetSide()];
        const Duo<int> &pSize = pattern.GetSize();

        // textúra X oszlopa
        int textureX = cast.WallX() * double(pSize.x);

        // bizonyos oldalak tükrözve jelennek meg, erre fix
        if (cast.GetSide() == 0 && rayDir.x < 0 || cast.GetSide() == 1 && rayDir.y > 0)
            textureX = pSize.x - textureX - 1;

        // nyújtás mértéke
        double scale = double(pSize.y) / lineHeight;

        // ha nem látszódik az egész textúra, a tetejét elhagyjuk
        double textureY = (drawStart - height / 2 + lineHeight / 2) * scale;

        // méretre nyújtjuk/zsugorítjuk a textúrát
        for (int y = drawStart; y < drawEnd; y++)
        {
            // textúra adott pixelét a képernyőre rajzoljuk9
            frameBuffer.SetPixel(x, y, pattern.GetPixel(textureX, textureY));
            textureY += scale;
        }

        // mélységi buffer a spriteokhoz
        zBuffer[x] = cast.GetWallDist();
    }

    // feloldás
    frameBuffer.UnLock();

    // rendererbe másolás
    SDL_RenderCopy(renderer, frameBuffer.GetTexture(), nullptr, nullptr);
}

void Window::DrawSprites(const Game &game)
{
    // aliases
    const Player &player = game.GetPlayer();

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
        sortedEnts[i].b = (player.GetPos() - sortedEnts[i].a->GetPos()).abssq();
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
        const Vector2 &dir = player.GetDir();
        const Vector2 plane = player.GetPlane();
        Texture &spriteTex = spriteTextures[ent.GetID()-1];
        const Duo<int> &spriteTexSize = spriteTex.GetSize();

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
        onTextureSource.x = (firstStripe - (-spriteSize / 2 + spriteScreenX)) * spriteTexSize.x / spriteSize;
        onTextureSource.w = (lastStripe - (-spriteSize / 2 + spriteScreenX)) * spriteTexSize.x / spriteSize - onTextureSource.x;
        onTextureSource.y = ((drawStart.y - height / 2 + spriteSize / 2) * spriteTexSize.y) / spriteSize;
        onTextureSource.h = ((drawEnd.y - height / 2 + spriteSize / 2) * spriteTexSize.y) / spriteSize - onTextureSource.y;

        SDL_RenderCopy(renderer, spriteTex.GetTexture(), &onTextureSource, &onScreenDestination);

        // known bug: placeholder flashes (suspicion: too small texture)
    }

    delete[] sortedEnts;
    frameBuffer.UnLock();
}

WindowInput::WindowInput() : Input() {}

void WindowInput::UpdateKeys(const SDL_KeyboardEvent &keyEvent)
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

void WindowInput::UpdateMouse(const SDL_MouseMotionEvent &mouseEvent)
{
    const double sensitivity = 0.5;
    turn += mouseEvent.xrel * sensitivity;
}

void WindowInput::SetShootTrigger()
{
    shootTrigger = true;
}