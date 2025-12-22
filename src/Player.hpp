#pragma once
#include <SFML/Graphics.hpp>

struct Player {
    sf::RectangleShape body;
    sf::Vector2f vel{ 0.f, 0.f };

    bool grounded = false;

    // wall
    bool touchingWall = false;
    int wallDir = 0;

    // dash
    bool canDash = true;
    bool dashing = false;
    float dashTimer = 0.f;

    // attack
    bool attacking = false;
    float attackTimer = 0.f;

    // slam
    bool slamming = false;

    int facing = 1;

    void respawn();
};
