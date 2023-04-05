#include "gamelogic.h"

Vector2 Player::plane() const{
    return Vector2(-dir.y, dir.x);
}

Player::Player(Vector2 position, Vector2 direction): pos(position), dir(direction) {}

Player::Player(const Player& p) : pos(p.pos), dir(p.dir) {}

Level::Level(const char* fileName){
    std::ifstream levelFile(fileName);

    if (!levelFile.is_open())
    {
        throw "WPO0HL";
    }
    // x sor
    // y oszlop
    // grid [x][y]
    levelFile >> sizex >> sizey;
    grid = new int*[sizex];
    for (int s = 0; s < sizex; s++)
    {
        grid[s] = new int[sizey];
        for (int o = 0; o < sizey; o++)
        {
            levelFile >> grid[s][o];
        }
    }

    levelFile.close();
}

Level::Level(const Level &l) : sizex(l.sizex), sizey(l.sizey) {
    grid = new int*[sizex];
    for (int s = 0; s < sizex; s++)
    {
        grid[s] = new int[sizey];
        for (int o = 0; o < sizey; o++)
        {
            grid[s][o] = l.grid[s][o];
        }
    }
}

Level::~Level(){
    for (int i = 0; i < sizex; i++)
        delete[] grid[i];
        
    delete[] grid;
}

int* Level::operator[](size_t index) const{
    return grid[index];
}

Input::Input(Vector2 dir, double turn) : dir(dir) , turn(turn) {}

Game::Game(Level gl, Player gp) : gLevel(gl), gPlayer(gp) {}