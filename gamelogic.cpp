#include "gamelogic.h"

Entity::Entity(int id, const Vector2& pos) : id(id), pos(pos) {}

Vector2 Player::plane() const
{
    return Vector2(-dir.y, dir.x) * 0.66;
}

Player::Player(Vector2 position, Vector2 direction) : Entity(-1, position), dir(direction) {}

Player::Player(const Player &p) : Entity(-1, p.pos), dir(p.dir) {}

void Entity::Move(const Matrix &grid, Vector2 moveDir, double deltaTime)
{
    moveDir.normalize();
    Vector2 nextPos = pos + moveDir * (deltaTime * .005);

    Ray path(grid, pos, moveDir);

    const double snapRange = .05;

    if ((path.end - pos).abs() - snapRange < (nextPos - pos).abs())
        pos += moveDir * (path.wallDist - snapRange);
    else
        pos = nextPos;
}

Input::Input(Vector2 dir, double turn) : dir(dir), turn(turn) {}

double Input::GetTurn()
{
    double re = turn;
    turn = 0;
    return re;
}

Game::Game(const char* saveName) {
    // open
    std::ifstream levelFile(saveName);
    if (!levelFile.is_open())
    {
        throw "WPO0HL";
    }

    // LOAD PLAYER
    levelFile >> player.pos.x >> player.pos.y;

    // LOAD LEVEL GRID
    // x sor
    // y oszlop
    // grid [x][y]
    int sizex = 0, sizey = 0;
    levelFile >> sizex >> sizey;
    int **grid = new int*[sizex];
    for (int s = 0; s < sizex; s++)
    {
        grid[s] = new int[sizey];
        for (int o = 0; o < sizey; o++)
        {
            levelFile >> grid[s][o];
        }
    }
    level.sizex = sizex;
    level.sizey = sizey;
    level.grid = grid;
    
    
    /*
    // LOAD ENTITIES
    levelFile >> entSize;
    for (int i = 0; i < entSize; i++)
    {
        int tempid, posx, posy;
        levelFile >> tempid >> posx >> posy;
        entities[i] = Entity(tempid, Vector2(posx, posy));
    }
    */


    // close
    levelFile.close();
}

void Game::SimulateGame(Input &inp, const double deltaTime)
{
    player.dir.rotate(inp.turn / 180);
    inp.turn = 0;

    Vector2 moveDir(player.dir * inp.dir.y + player.plane() * inp.dir.x);

    player.Move(level, moveDir, deltaTime);
}