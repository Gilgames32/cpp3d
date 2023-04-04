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

void Window::DrawMinimap(const Level& lev){

    for (int i = 0; i < lev.sizex; i++)
    {
        for (int j = 0; j < lev.sizey; j++)
        {
            if (lev[i][j] != 0)
            {
                rectangleColor(renderer, i*10, j*10, (i+1)*10, (j+1)*10, 0xFFFFFFFF);
            }
        }
        
    }

    //circleColor(renderer, pPos.x*10, pPos.y*10, 2, 0xFF0000FF);
}

void Window::DrawPerspective(const Level& lev, const Player& pla){
    
    Vector2 plane = pla.plane();
    for (int i = 0; i < width; i+=1)
    {
        //kiindulási cella cella
        pair<int> rayCell = pair<int>(pla.pos.x, pla.pos.y);

        //jelenlegi csík kamerához relatív aránya -1...1
        double camX = 2*double(i)/width - 1;

        //sugár irányvektora
        Vector2 rayDir = pla.dir + plane * camX;

        //rácsvonalanként léptetett pont
        Vector2 sideDist;

        //egyik x vagy y oldalról a legközelebbi átellenes oldalig a távolság
        //1 helyett átfogó kéne
        Vector2 deltaDist = Vector2(abs(1/rayDir.x), abs(1/rayDir.y));

        //lépegeté irányának előjele, -1 vagy 1
        pair<int> stepDir;
        stepDir.x = rayDir.x < 0 ? -1 : 1;
        stepDir.y = rayDir.y < 0 ? -1 : 1;

        //kamera síkja és a fal közti távolság
        double wallDist;

        //vízszintes / függőleges oldalt talált el
        bool side;


        //legközelebbi falig távolság
        if(rayDir.x < 0)
            sideDist.x = (pla.pos.x - rayCell.x) * deltaDist.x;
        else
            sideDist.x = (rayCell.x + 1.0 - pla.pos.x) * deltaDist.x;

        if(rayDir.y < 0)
            sideDist.y = (pla.pos.y - rayCell.y) * deltaDist.y;
        else
            sideDist.y = (rayCell.y + 1.0 - pla.pos.y) * deltaDist.y;

        while (lev[rayCell.x][rayCell.y] == 0)
        {
            if(sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                rayCell.x += stepDir.x;
                side = false;
            }
            else
            {
                sideDist.y += deltaDist.y;
                rayCell.y += stepDir.y;
                side = true;
            }
        }

        if(side == 0) wallDist = (sideDist.x - deltaDist.x);
        else          wallDist = (sideDist.y - deltaDist.y);
        
        int lineHeight = (int)(heigth*.5 / wallDist);
        int drawStart = -lineHeight / 2 + heigth / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + heigth / 2;
        if(drawEnd >= heigth) drawEnd = heigth - 1;

        lineColor(renderer, i, drawStart, i, drawEnd, side ? 0x888888FF : 0xFFFFFFFF);
        Vector2 hit = pla.pos + rayDir * wallDist;
        lineColor(renderer, hit.x * 10, hit.y * 10, pla.pos.x * 10, pla.pos.y * 10, 0x0000FFFF);
    }
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