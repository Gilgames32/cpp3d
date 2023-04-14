#include "graphics.h"

Window::Window(int w, int h) : width(w), heigth(h) //pattern("grass_side.png"), frameBuffer(w, h, SDL_PIXELFORMAT_ABGR8888)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    // textures initialize first but theyd need a renderer, retard

    window = SDL_CreateWindow("NHZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_SHOWN);
    
    Texture::windowRenderer = renderer;
    pattern = new Texture("rolopipi.png");
    frameBuffer = new Texture(w, h, SDL_PIXELFORMAT_ABGR8888);

    Uint32 wpf;
    SDL_QueryTexture(frameBuffer->texture, &wpf, nullptr, nullptr, nullptr);
    Texture::windowFormat = wpf;
    std::cout << SDL_GetPixelFormatName(frameBuffer->format)<< std::endl;
    std::cout << SDL_GetPixelFormatName(pattern->format)<< std::endl;

    // SDL_SetWindowGrab(window, SDL_TRUE);
    
    // trap mouse uwu
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

Window::~Window()
{
    delete pattern;
    delete frameBuffer;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::Clear(){
    frameBuffer->Lock();
    frameBuffer->Clear();
    frameBuffer->UnLock();
    
    

    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

}

void Window::Render(){
    SDL_RenderCopy(renderer, frameBuffer->texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Window::DrawMinimap(const Game& game){

    for (int i = 0; i < game.gLevel.sizex; i++)
    {
        for (int j = 0; j < game.gLevel.sizey; j++)
        {
            if (game.gLevel[i][j] != 0)
            {
                rectangleColor(renderer, i*10, j*10, (i+1)*10, (j+1)*10, 0xFFFFFFFF);
            }
        }
        
    }
    circleColor(renderer, game.gPlayer.pos.x*10, game.gPlayer.pos.y*10, 2, 0xFF0000FF);
}

void Window::DrawPerspective(const Game& game){
    frameBuffer->Lock();

    Vector2 plane = game.gPlayer.plane();
    for (int x = 0; x < width; x++)
    {
        // jelenlegi csík kamerához relatív aránya -1...1
        double camX = 2*double(x)/width - 1;

        // sugár irányvektora
        Vector2 rayDir = game.gPlayer.dir + plane * camX;
        
        // a sugár
        Ray cast = Ray(game.gLevel, game.gPlayer.pos, rayDir);

        // a fal magassága:
        int lineHeight = heigth / cast.wallDist;
        int drawStart = -lineHeight / 2 + heigth / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2  + heigth / 2;
        if(drawEnd >= heigth) drawEnd = heigth - 1;

        
        // textúra X oszlopa
        int textureX = cast.WallX() * double(pattern->width);
        if(cast.side == 0 && cast.dir.x > 0) textureX = pattern->width - textureX - 1;
        if(cast.side == 1 && cast.dir.y < 0) textureX = pattern->width - textureX - 1;

        // nyújtás mértéke
        double scale = double(pattern->height) / lineHeight;

        // ha a textúra lelógna, nem a tetején kezdjük kirajzolni
        double texturePos = (drawStart - heigth / 2 + lineHeight / 2) * scale;


        for(int y = drawStart; y<drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int textureY = texturePos;
            texturePos += scale;
            Uint32 pixel = pattern->GetPixel(textureX, textureY);
            
            // igény szerint sötétítés
            if(cast.side) pixel = (pixel >> 1) & 0xFF7F7F7F;
            frameBuffer->SetPixel(x, y, pixel);

            // !! https://gamedev.stackexchange.com/questions/102490/fastest-way-to-render-image-data-from-buffer
        }

        // fal
        // lineColor(renderer, x, drawStart, x, drawEnd, cast.side ? 0x888888FF : 0xFFFFFFFF);
        // minimap debug
        lineColor(renderer, cast.end.x * 10, cast.end.y * 10, cast.start.x * 10, cast.start.y * 10, 0x0000FFFF);
    }

    frameBuffer->UnLock();
}


WindowInput::WindowInput() : Input() {}

void WindowInput::UpdateKeys(SDL_KeyboardEvent keyEvent){
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
    
    dir = Vector2(d-a, w-s);
}

void WindowInput::UpdateMouse(SDL_MouseMotionEvent mouseEvent){
    turn += mouseEvent.xrel; // multiply by sensitivity latur
}