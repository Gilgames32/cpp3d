#include "gamelogic.h"

Entity::Entity(int id, const Vector2& pos) : id(id), pos(pos) {}
Entity::Entity(const Entity& e) : id(e.id), pos(e.pos) {}
void Entity::operator=(const Entity& e) 
{
    id = e.id;
    pos = e.pos;
}

void Entity::Move(const Matrix &grid, Vector2 moveDir, double deltaTime, double speed)
{
    // egységnyi hosszúságúra nyújtjuk
    moveDir = moveDir.normalize();

    // kívánt következő pozíció
    Vector2 nextPos = pos + moveDir * (deltaTime*speed/1000);

    // raycastoljuk, hogy vezet-e oda falmentes út
    Ray path(grid, pos, moveDir);

    // tetszőleges pontossággal megközelítjük a falat
    double snap = 0.01;
    double maxDist = path.wallDist - snap;
    if (maxDist < 0) maxDist = 0;
    double movDist = (pos - nextPos).abs();

    // ha a becsapódás közelebb van mint a megtenni kívánt út, az azt jelenti hogy falba ütköznénk
    if (movDist > maxDist)
    {
        // falig
        pos += moveDir * maxDist;

        // maradék momentum átalakítása csúszásba
        Vector2 slide = nextPos - pos - moveDir * maxDist;
        // attól függően hogy melyik oldalon csúszunk elhagyjuk az egyik komponenst
        if (path.side)
            slide.y = 0;
        else
            slide.x = 0;

        // ezt is raycastoljuk
        Ray slidePath(grid, pos, slide.normalize());
        double slideMax = slidePath.wallDist - snap;
        if (slideMax < 0) slideMax = 0;
        double slideDist = slide.abs();
        if (slideDist > slideMax)
            pos += slide.normalize() * slideMax;
            // ezt már nincs értelme tovább slideolni mert merőleges
        else
            pos += slide;
    }
    else
    {
        pos = nextPos;
    }
}

Vector2 Player::GetPlane() const
{
    return Vector2(-dir.y, dir.x) * 0.66; // fov
}

Player::Player(const Vector2& position, const Vector2& direction) : Entity(-1, position), dir(direction) {}

Player::Player(const Player &p) : Entity(-1, p.pos), dir(p.dir) {}

bool Player::Shoot(const Matrix &level, DinTomb<Entity>& entities)
{
    // tekintsük a lövést egy szakasznak
    // ha 1/2 egységen belül van az találat (egy egység széles spriteok)
    // ezek közül a legközelebbi számí találatnak, nincs piercing
    std::cout << "shoot-";

    // a lövés csíkja
    Ray trail(level, pos, dir);

    // találatok tömbje
    DinTomb<Pair<size_t, double>> hits;
    for (auto i = entities.begin(); i != entities.end(); ++i)
    {        
        bool perp;
        Vector2 closest;
        double dist = Vector2::PointSegDist(trail.start, trail.end, i->pos, perp, closest);
        // ha van rá merőleges és el is találta
        if (perp && dist < .5)
            hits.Append(Pair<size_t, double>(i.Index(), (closest - pos).abs()));
    }

    // ha van találat
    if (hits.Size() == 0)
    {
        std::cout << "miss" << std::endl;
        return false;
    }

    // legközelebbi indexe
    size_t mindex = 0;
    for (auto i = hits.begin(); i != hits.end(); ++i)
        if (i->b < hits[mindex].b)
            mindex = i.Index();

    entities.Delete(hits[mindex].a);
    std::cout << "hit" << std::endl;
    return true;
}

void Player::DecreaseCoolDowns(double deltaTime)
{
    if (shootCoolDown > 0)
    {
        shootCoolDown -= deltaTime;
        if (shootCoolDown < 0)
            shootCoolDown = 0;
    }

    if (damageCoolDown > 0)
    {
        damageCoolDown -= deltaTime;
        if (damageCoolDown < 0)
            damageCoolDown = 0;
    }
    
}

bool Player::DamagePlayer(int damage)
{
    if (damageCoolDown > 0)
        return false;
    
    const double damageCoolDownMax = 1000;
    damageCoolDown = damageCoolDownMax;
    
    health -= damage;
    std::cout << damageCoolDown << " " << health << "\n";
    return health <= 0;
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
    int entSize;
    levelFile >> entSize;
    for (int i = 0; i < entSize; i++)
    {
        int tempid, posx, posy;
        levelFile >> tempid >> posx >> posy;
        ent.Append(Entity(tempid, Vector2(posx, posy)));
    }



    // close
    levelFile.close();
}

Game::~Game()
{
    //delete[] entities;
}

bool Game::SimulateGame(Input &inp, double deltaTime)
{
    // decrease cooldowns
    player.DecreaseCoolDowns(deltaTime);
    
    // process inputs
    player.dir.rotate(inp.GetTurn() / 180);
    if (inp.dir != Vector2(0, 0))
    {
        Vector2 moveDir(player.dir * inp.dir.y + player.GetPlane() * inp.dir.x);
        player.Move(level, moveDir, deltaTime, 3);
    }

    // shooting
    if (inp.GetShootTrigger())
        player.Shoot(level, ent);

    // process entities
    for (auto i = ent.begin(); i != ent.end(); ++i)
    {
        // ha látja a playert
        Ray view(level, i->pos, player.pos - i->pos);
        double const playerDistance = (player.pos - view.start).abs();
        if ((view.end - view.start).abs() > playerDistance)
        {
            // mozgat ha nincs túl közel
            const double followStopDist = 0.2;
            const double damageDist = 0.2;
            if (playerDistance > followStopDist)
            {
                i->Move(level, player.pos - view.start, deltaTime);
            }
            if (playerDistance < damageDist)
            {
                if(player.DamagePlayer(10))
                    return true;
            }
        }
    }
    return false;
}