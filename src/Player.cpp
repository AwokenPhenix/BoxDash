#include "Player.hpp"
#include "Config.hpp"

void Player::respawn() {
    body.setPosition(SPAWN_POINT);
    vel = { 0.f, 0.f };

    grounded = false;
    touchingWall = false;
    wallDir = 0;

    canDash = true;
    dashing = false;
    dashTimer = 0.f;

    attacking = false;
    attackTimer = 0.f;

    slamming = false;   
    facing = 1;
}
