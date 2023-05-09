#include "gamelogic.h"

Entity::Entity(int id, const Vector2& pos) : id(id), pos(pos) {}

Vector2 Player::plane() const
{
    return Vector2(-dir.y, dir.x) * 0.66; // fov
}

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

Player::Player(const Vector2& position, const Vector2& direction) : Entity(-1, position), dir(direction) {}

Player::Player(const Player &p) : Entity(-1, p.pos), dir(p.dir) {}

bool Player::Shoot(const Matrix &level, Entity* entities, int entSize)
{
    // i mean it works but this is ugly as hell man


    
    // optimalizálás:
        // csak enemy spriteokra
        // closest to furthest !!! closest point számít nem entpos
        // minkeres sort helyett
    // van egy szakaszunk
    // szakasz és pont távolsága, ha 1/2 alatti, death
    std::cout << "shoot-";


    // a lövés csíkja
    Ray trail(level, pos, dir);

    // sort and break on find
    // filter ones in range
    int hitSize = 0;
    // .a: entity pointer   .b: dist between closest point and player
    Pair<Entity *, double> *sortedHits = new Pair<Entity *, double>[entSize];
    for (int i = 0; i < entSize; i++)
    {
        // reset sprite (degugging)
        entities[i].id = 0;
        
        bool perp;
        Vector2 closest;
        double dist = Vector2::PointSegDist(trail.start, trail.end, entities[i].pos, perp, closest);

        if (perp && dist < .5)
        {
            entities[i].id = 1;
            sortedHits[hitSize++] = Pair<Entity *, double>(entities + i, (closest - pos).abs());
        }
    }
    if (hitSize == 0)
    {
        std::cout << "miss" << std::endl;
        return false;
    }
    

    // minkeres
    int mindex = 0;
    for (int i = 0; i < hitSize; i++)
    {
        if (sortedHits[i].b < sortedHits[mindex].b)
        {
            mindex = i;
        }
    }
    sortedHits[mindex].a->id = 2;
    std::cout << "hit" << std::endl;
    return true;
}

Input::Input(const Vector2& dir, double turn) : dir(dir), turn(turn) {}

double Input::GetTurn()
{
    double re = turn;
    turn = 0;
    return re;
}

bool Input::GetShootTrigger() 
{
    bool re = shootTrigger;
    shootTrigger = false;
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
    
    

    // LOAD ENTITIES
    levelFile >> entSize;
    entities = new Entity[entSize];
    for (int i = 0; i < entSize; i++)
    {
        int tempid, posx, posy;
        levelFile >> tempid >> posx >> posy;
        entities[i] = Entity(tempid, Vector2(posx, posy));
    }



    // close
    levelFile.close();
}

void Game::SimulateGame(Input &inp, double deltaTime)
{
    player.dir.rotate(inp.GetTurn() / 180);

    Vector2 moveDir(player.dir * inp.dir.y + player.plane() * inp.dir.x);

    player.Move(level, moveDir, deltaTime);
    if (inp.GetShootTrigger())
    {
        player.Shoot(level, entities, entSize);
    }
    
}