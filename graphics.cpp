#include "graphics.h"

Window::Window(int w, int h)
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

void Window::DrawMinimap(const Level& griddy){

    for (int i = 0; i < griddy.sizex; i++)
    {
        for (int j = 0; j < griddy.sizey; j++)
        {
            if (griddy.grid[i][j] != 0)
            {
                rectangleColor(renderer, i*10, j*10, (i+1)*10, (j+1)*10, 0xFFFFFFFF);
            }
        }
        
    }

    //circleColor(renderer, pPos.x*10, pPos.y*10, 2, 0xFF0000FF);
}


Input::Input(Vector2 dir) : dir(dir) {}

void Input::UpdateKeys(SDL_KeyboardEvent keyEvent){
    switch (keyEvent.keysym.sym)
    {
    case SDLK_ESCAPE:
        // unlock mouse
        SDL_SetRelativeMouseMode(SDL_FALSE);

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

void Input::UpdateMouse(SDL_MouseMotionEvent mouseEvent){
    turn += mouseEvent.xrel; // multiply by sensitivity latur
}