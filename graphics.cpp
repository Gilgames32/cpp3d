#include "graphics.h"

Window::Window(int w, int h) : width(w), heigth(h)
{
    SDL_Init(SDL_INIT_EVERYTHING);


    window = SDL_CreateWindow("UWU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    // SDL_SetWindowGrab(window, SDL_TRUE);
    
    // trap mouse uwu
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::Clear(){
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Window::Render(){
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
    
    Vector2 plane = game.gPlayer.plane();
    for (int i = 0; i < width; i+=1)
    {
        //jelenlegi csík kamerához relatív aránya -1...1
        double camX = 2*double(i)/width - 1;

        //sugár irányvektora
        Vector2 rayDir = game.gPlayer.dir + plane * camX;
        
        Ray cast = Ray(game.gLevel, game.gPlayer.pos, rayDir);


        int lineHeight = (int)(heigth*.5 / cast.wallDist);
        int drawStart = -lineHeight / 1 + heigth / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 1  + heigth / 2;
        if(drawEnd >= heigth) drawEnd = heigth - 1;

        lineColor(renderer, i, drawStart, i, drawEnd, cast.side ? 0x888888FF : 0xFFFFFFFF);
        Vector2 hit = game.gPlayer.pos + rayDir * cast.wallDist;
        lineColor(renderer, hit.x * 10, hit.y * 10, game.gPlayer.pos.x * 10, game.gPlayer.pos.y * 10, 0x0000FFFF);
    }
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