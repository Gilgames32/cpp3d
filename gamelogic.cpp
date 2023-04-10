#include "gamelogic.h"

Vector2 Player::plane() const{
    return Vector2(-dir.y, dir.x)*0.66;
}

Player::Player(Vector2 position, Vector2 direction): pos(position), dir(direction) {}

Player::Player(const Player& p) : pos(p.pos), dir(p.dir) {}

void Player::Move(const Level& grid, Vector2 moveDir, double deltaTime){
    moveDir.normalize();
    Vector2 nextPos = pos + moveDir * (deltaTime * .005);
    
    Ray path(grid, pos, moveDir);

    const double snapRange = .05;

    if ((path.end - pos).abs() - snapRange < (nextPos - pos).abs())
        pos += moveDir * (path.wallDist - snapRange);
    else
        pos = nextPos;
    
    
    
     
}

Input::Input(Vector2 dir, double turn) : dir(dir) , turn(turn) {}

double Input::GetTurn() {
    double re = turn;
    turn = 0;
    return re;
}

Game::Game(Level gl, Player gp) : gLevel(gl), gPlayer(gp) {}

void Game::SimulateGame(Input& inp, const double deltaTime) {
    gPlayer.dir.rotate(inp.turn / 180);
    inp.turn = 0;

    Vector2 moveDir(gPlayer.dir * inp.dir.y + gPlayer.plane() * inp.dir.x);

    gPlayer.Move(gLevel, moveDir, deltaTime);
}