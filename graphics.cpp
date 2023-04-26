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

    spriteTextures.AddTexture("./ass/placeholder.png");





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
    // SDL_RenderCopy(renderer, frameBuffer.texture, nullptr, nullptr);

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

void Window::DrawSprites(const Game &game)
{
    frameBuffer.Lock();
    // sortable array of entities
    pair<Entity*, double> *sortedEnts = new pair<Entity*, double>[game.entSize];
    
    // distance between camera  and sprites
    for (int i = 0; i < game.entSize; i++)
    {
        sortedEnts[i].a = game.entities + i;
        Vector2 distance = game.player.pos - sortedEnts[i].a->pos;
        sortedEnts[i].b = distance.abs();
    }

    // bubble sort by distance
    for (int i = 0; i < game.entSize - 1; i++)
        for (int j = 0; j < game.entSize - i - 1; j++)
            if (sortedEnts[j].b < sortedEnts[j+1].b)
            {
                pair<Entity*, double> temp = sortedEnts[j];
                sortedEnts[j] = sortedEnts[j+1];
                sortedEnts[j+1] = temp;
            }
    
    // ended here
    /*
    4: Project the sprite on the camera plane (in 2D): subtract the player position from the sprite position, then multiply the result with the inverse of the 2x2 camera matrix
    5: Calculate the size of the sprite on the screen (both in x and y direction) by using the perpendicular distance
    6: Draw the sprites vertical stripe by vertical stripe, don't draw the vertical stripe if the distance is further away than the 1D ZBuffer of the walls of the current stripe
    7: Draw the vertical stripe pixel by pixel, make sure there's an invisible color or all sprites would be rectangles
    */
    //shu

    // project it to the camera
    for (int i = 0; i < game.entSize; i++)
    {
        // aliases
        Entity& ent = *(sortedEnts[i].a);
        Vector2 dir = game.player.dir;
        Vector2 plane = game.player.plane();

        // relative position to camera
        Vector2 relativeEntPos = ent.pos - game.player.pos;

        // transform sprite with the inverse camera matrix
        //  [ planeX   dirX ] ^-1                                      [ dirY      -dirX ]
        //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        //  [ planeY   dirY ]                                          [ -planeY  planeX ]

        // mátrix determiniánsának reciproka
        double detRec = 1.0 / (plane.x * dir.y - dir.x * plane.y);

        // sprites position in camera space
        Vector2 transform;
        transform.x = detRec * (dir.y * relativeEntPos.x - dir.x * relativeEntPos.y);
        // mivel a kamera síkjára merőleges ezért igazából egy kamerától való távolságot jelent
        transform.y = detRec * (-plane.y * relativeEntPos.x + plane.x * relativeEntPos.y);

        // the center of the sprite on the screen
        int spriteScreenX = int((width / 2) * (1 + transform.x / transform.y));
        // debugging
        circleColor(renderer, spriteScreenX, heigth/2, 6, 0xFFFFFFFF);

        
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