#include "gamelogic.h"

Vector2 Player::plane() const{
    return Vector2(-dir.y, dir.x);
}

Player::Player(Vector2 position, Vector2 direction): pos(position), dir(direction) {}

Player::Player(const Player& p) : pos(p.pos), dir(p.dir) {}

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

    gPlayer.pos += (gPlayer.dir * inp.dir.y + gPlayer.plane() * inp.dir.x) * (deltaTime / 500);
}