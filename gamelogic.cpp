#include "gamelogic.h"
// #define CPORTA

Entity::Entity(int id, const Vector2 &pos) : id(id), pos(pos) {}

int Entity::GetID() const { return id; }

const Vector2 &Entity::GetPos() const { return pos; }

void Entity::Move(const Matrix &grid, Vector2 moveDir, double deltaTime, double speed)
{
    // egységnyi hosszúságúra nyújtjuk
    try
    {
        moveDir.normalize();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    moveDir = moveDir.normalize();

    // kívánt következő pozíció
    Vector2 nextPos = pos + moveDir * (deltaTime * speed / 1000);

    // raycastoljuk, hogy vezet-e oda falmentes út
    Ray path(grid, pos, moveDir);

    // tetszőleges pontossággal megközelítjük a falat
    double snap = 0.01;
#ifdef CPORTA
    snap = 0;
#endif
    double maxDist = (pos - path.GetEnd()).abs() - snap;
    if (maxDist < 0)
        maxDist = 0;
    double movDist = (pos - nextPos).abs();

    // ha a becsapódás közelebb van mint a megtenni kívánt út, az azt jelenti, hogy falba ütköznénk
    if (movDist > maxDist)
    {
        // falig
        pos += moveDir * maxDist;

        // maradék momentum átalakítása csúszásba
        Vector2 slide = nextPos - pos - moveDir * maxDist;
        // attól függően, hogy melyik oldalon csúszunk elhagyjuk az egyik komponenst
        if (path.GetSide())
            slide.y = 0;
        else
            slide.x = 0;

        // ha kell tovább csúsztatni
        if (slide != Vector2(0, 0))
        {
            try
            {
                // ezt is raycastoljuk
                Ray slidePath(grid, pos, slide.normalize());
                double slideMax = slidePath.GetWallDist() - snap;
                if (slideMax < 0)
                    slideMax = 0;
                double slideDist = slide.abs();
                if (slideDist > slideMax)
                    pos += slide.normalize() * slideMax;
                // ezt már nincs értelme tovább slideolni mert merőleges
                else
                    pos += slide;
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
    else
    {
        pos = nextPos;
    }
}

Player::Player(const Vector2 &position, const Vector2 &direction) : Entity(-1, position), dir(direction) {}

Player::Player(const Player &p) : Entity(-1, p.GetPos()), dir(p.dir) {}

int Player::GetHP() const { return health; }

const Vector2 &Player::GetDir() const { return dir; }

Vector2 Player::GetPlane() const { return Vector2(-dir.y, dir.x) * 0.66; } // szorzóval állítható az FOV

void Player::Rotate(double turn) { dir.rotate(turn); }

bool Player::Shoot(const Matrix &level, DinTomb<Entity> &entities)
{
    if (shootCoolDown > 0)
        return false;
    const double shootDelay = 180;
    shootCoolDown = shootDelay;

    // tekintsük a lövést egy szakasznak
    // ha 1/2 egységen belül van az találat (egy egység széles spriteok)
    // ezek közül a legközelebbi számí találatnak, nincs piercing

    // a lövés csíkja
    Ray trail(level, pos, dir);

    // találatok tömbje
    DinTomb<Pair<size_t, double>> hits;
    for (auto i = entities.begin(); i != entities.end(); ++i)
    {
        bool perp;
        Vector2 closest;
        double dist = Vector2::PointSegDist(trail.GetStart(), trail.GetEnd(), i->GetPos(), perp, closest);
        // ha van rá merőleges és el is találta
        if (perp && dist < .5)
            hits.Append(Pair<size_t, double>(i.Index(), (closest - pos).abssq()));
    }

    // ha van találat
    if (hits.Size() == 0)
    {
        return false;
    }

    // legközelebbi indexe
    size_t mindex = 0;
    for (auto i = hits.begin(); i != hits.end(); ++i)
        if (i->b < hits[mindex].b)
            mindex = i.Index();

    // just for good measures
    try
    {
        entities.Delete(hits[mindex].a);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

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

    const double damageDelay = 500;
    damageCoolDown = damageDelay;

    health -= damage;
    return health <= 0;
}

Input::Input(const Vector2 &dir, double turn, bool shoot) : turn(turn), shootTrigger(shoot), dir(dir) {}

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

const Vector2 &Input::GetDir() const { return dir; }

Game::Game(const char *saveName)
{
    // open
    std::ifstream levelFile(saveName);
    if (!levelFile.is_open())
    {
        throw std::runtime_error("Hiba a fájl megnyitásakor");
    }

    // load player
    double px, py;
    levelFile >> px >> py;
    player = Player(Vector2(px + .5, py + .5));

    // load grid
    // x sor
    // y oszlop
    // grid [x][y]
    int sizex = 0, sizey = 0;
    levelFile >> sizex >> sizey;
    int **grid = new int *[sizex];
    for (int s = 0; s < sizex; s++)
    {
        grid[s] = new int[sizey];
        for (int o = 0; o < sizey; o++)
        {
            levelFile >> grid[s][o];
        }
    }
    level = Matrix(sizex, sizey, grid);
    for (int i = 0; i < sizex; i++)
    {
        delete[] grid[i];
    }
    delete[] grid;

    // load entities
    int entSize;
    levelFile >> entSize;
    for (int i = 0; i < entSize; i++)
    {
        int tempid;
        double posx, posy;
        levelFile >> tempid >> posx >> posy;
        entities.Append(Entity(tempid, Vector2(posx + .5, posy + .5)));
    }

    // close
    levelFile.close();
}

const Matrix &Game::GetLevel() const { return level; }

const Player &Game::GetPlayer() const { return player; }

const DinTomb<Entity> &Game::GetEntities() const { return entities; }

bool Game::SimulateGame(Input &inp, double deltaTime)
{
    // decrease cooldowns
    player.DecreaseCoolDowns(deltaTime);

    // process inputs
    player.Rotate(inp.GetTurn() / 180);
    if (inp.GetDir() != Vector2(0, 0))
    {
        Vector2 moveDir(player.GetDir() * inp.GetDir().y + player.GetPlane().normalize() * inp.GetDir().x);
        player.Move(level, moveDir, deltaTime, 3);
    }

    // shooting
    if (inp.GetShootTrigger())
    {
        player.Shoot(level, entities);
    }

    // process entities
    if (entities.Size() <= 0)
    {
        return true;
    }

    for (auto i = entities.begin(); i != entities.end(); ++i)
    {
        // aliases
        const Vector2 &enemyPos = i->GetPos();
        const Vector2 &playerPos = player.GetPos();

        // ha látja a playert
        Ray view(level, enemyPos, playerPos - enemyPos);
        double const playerDistance = (playerPos - enemyPos).abssq();
        if ((view.GetEnd() - view.GetStart()).abssq() > playerDistance)
        {
            // mozgat ha nincs túl közel
            const double followStopDist = 0.2;
            const double damageDist = 0.2;
            if (playerDistance > followStopDist)
            {
                i->Move(level, playerPos - view.GetStart(), deltaTime);
            }
            if (playerDistance < damageDist)
            {
                if (player.DamagePlayer(10))
                {
                    return true;
                }
            }
        }
    }
    return false;
}