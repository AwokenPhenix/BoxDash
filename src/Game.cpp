#include "Game.hpp"
#include "Config.hpp"
#include <cmath>

static float clampf(float v, float lo, float hi) {
    return std::max(lo, std::min(v, hi));
}

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(960, 540)), "Platformer"),
    view({ 0,0 }, { VIEW_W, VIEW_H })
{
    window.setFramerateLimit(60);
    window.setView(view);

    player.body.setSize({ PLAYER_SIZE, PLAYER_SIZE });
    player.body.setFillColor(sf::Color(0, 255, 180));
    player.respawn();

    level.build();

    // enemies
    for (int i = 0; i < 4; ++i) {
        Enemy e;
        e.body.setSize({ 28.f, 28.f });
        e.body.setFillColor(sf::Color::Red);
        e.body.setPosition({ 600.f + i * 60.f, GROUND_Y - 28.f });
        enemies.push_back(e);
    }

    attackBox.setSize({ 24.f, 28.f });
    attackBox.setFillColor(sf::Color(255, 255, 255, 200));
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clampf(clock.restart().asSeconds(), 0.f, 0.05f);
        processInput(dt);
        update(dt);
        render();
    }
}
#include "Game.hpp"
#include "Config.hpp"
#include <SFML/Window/Event.hpp>

// ------------------------------------------------
// Input
// ------------------------------------------------
void Game::processInput(float /*dt*/) {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}


// ------------------------------------------------
// Update (TEMP: does nothing yet)
// ------------------------------------------------
void Game::update(float dt) {
    // ---------------- HITSTOP ----------------
    if (hitstopTimer > 0.f) {
        hitstopTimer -= dt;
        return;
    }

    // ---------------- INPUT ----------------
    float move = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move += 1.f;

    if (move != 0.f)
        player.facing = (move > 0.f ? 1 : -1);

    bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    bool dashPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    bool slamPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
    bool attackPressed =
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // ---------------- NORMAL ATTACK ----------------
    if (attackPressed && !player.attacking) {
        player.attacking = true;
        player.attackTimer = ATTACK_TIME;
    }

    if (player.attacking) {
        player.attackTimer -= dt;
        if (player.attackTimer <= 0.f)
            player.attacking = false;
    }

    // ---------------- SLAM (SEPARATE MECHANIC) ----------------
    if (!player.grounded && slamPressed && !player.slamming) {
        player.slamming = true;
        player.vel.x = 0.f;
        player.vel.y = SLAM_SPEED;
    }

    // ---------------- JUMP / WALL JUMP ----------------
    if (jumpPressed && !player.slamming) {
        if (player.grounded) {
            player.vel.y = JUMP_VEL;
            player.grounded = false;
        }
        else if (player.touchingWall) {
            player.vel.y = JUMP_VEL;
            player.vel.x = -player.wallDir * WALL_JUMP_X;
            player.touchingWall = false;
        }
    }

    // ---------------- DASH ----------------
    if (dashPressed && player.canDash && !player.dashing && !player.slamming) {
        player.dashing = true;
        player.canDash = false;
        player.dashTimer = DASH_TIME;
        player.vel = { DASH_SPEED * (float)player.facing, 0.f };
    }

    if (player.dashing) {
        player.dashTimer -= dt;
        if (player.dashTimer <= 0.f)
            player.dashing = false;
    }

    // ---------------- HORIZONTAL SPEED ----------------
    if (!player.dashing && !player.slamming) {
        player.vel.x = move * MOVE_SPEED;
    }

    // ---------------- GRAVITY ----------------
    if (!player.dashing && !player.slamming)
        player.vel.y += GRAVITY * dt;

    // ==================================================
    // MOVE Y — STABLE COLLISION
    // ==================================================
    player.body.move({ 0.f, player.vel.y * dt });
    player.grounded = false;

    sf::FloatRect pb = player.body.getGlobalBounds();
    for (auto& plat : level.platforms) {
        sf::FloatRect wb = plat.getGlobalBounds();
        if (pb.findIntersection(wb)) {
            if (player.vel.y > 0.f) {
                player.body.setPosition({
                    player.body.getPosition().x,
                    wb.position.y - PLAYER_SIZE
                    });
                player.vel.y = 0.f;
                player.grounded = true;
                player.canDash = true;

                if (player.slamming) {
                    player.slamming = false;
                }
            }
            else if (player.vel.y < 0.f) {
                player.body.setPosition({
                    player.body.getPosition().x,
                    wb.position.y + wb.size.y
                    });
                player.vel.y = 0.f;
            }
        }
    }



    // ==================================================
    // MOVE X — STABLE COLLISION
    // ==================================================
    player.body.move({ player.vel.x * dt, 0.f });

    // ---------------- SLAM  ENEMY HIT (CORRECT TIMING) ----------------
    if (player.slamming && player.vel.y > 0.f) {
        sf::FloatRect playerBox = player.body.getGlobalBounds();

        for (auto& e : enemies) {
            if (e.alive &&
                playerBox.findIntersection(e.body.getGlobalBounds())) {

                e.alive = false;
                hitstopTimer = HITSTOP_TIME;
            }
        }
    }

    player.touchingWall = false;
    player.wallDir = 0;

    pb = player.body.getGlobalBounds();
    for (auto& plat : level.platforms) {
        sf::FloatRect wb = plat.getGlobalBounds();
        if (pb.findIntersection(wb)) {
            if (player.vel.x > 0.f) {
                player.body.setPosition({
                    wb.position.x - PLAYER_SIZE,
                    player.body.getPosition().y
                    });
                player.vel.x = 0.f;
                player.touchingWall = true;
                player.wallDir = 1;
            }
            else if (player.vel.x < 0.f) {
                player.body.setPosition({
                    wb.position.x + wb.size.x,
                    player.body.getPosition().y
                    });
                player.vel.x = 0.f;
                player.touchingWall = true;
                player.wallDir = -1;
            }
        }
    }

    // ---------------- WALL SLIDE ----------------
    if (player.touchingWall && !player.grounded &&
        player.vel.y > WALL_SLIDE_SPEED) {
        player.vel.y = WALL_SLIDE_SPEED;
    }

    // ---------------- NORMAL ATTACK HITBOX ----------------
    if (player.attacking) {
        attackBox.setPosition({
            player.body.getPosition().x + player.facing * PLAYER_SIZE,
            player.body.getPosition().y + 2.f
            });

        for (auto& e : enemies) {
            if (e.alive &&
                attackBox.getGlobalBounds().findIntersection(e.body.getGlobalBounds())) {

                e.alive = false;
                player.canDash = true;
                hitstopTimer = HITSTOP_TIME;
            }
        }
    }

    // ---------------- FALL RESET ----------------
    if (player.body.getPosition().y > FALL_DEATH_Y) {
        player.respawn();
    }

    // ---------------- CAMERA FOLLOW ----------------
    sf::Vector2f pc =
        player.body.getPosition() + player.body.getSize() * 0.5f;

    sf::Vector2f cc = view.getCenter();
    float t = 1.f - std::exp(-8.f * dt);
    view.setCenter(cc + (pc - cc) * t);
    window.setView(view);
}





// ------------------------------------------------
// Render
// ------------------------------------------------
void Game::render() {
    window.clear(sf::Color(15, 15, 40));

    // draw level
    for (auto& p : level.platforms) {
        window.draw(p);
    }

    // draw enemies
    for (auto& e : enemies) {
        if (e.alive)
            window.draw(e.body);
    }

    if (player.attacking) {
        window.draw(attackBox);
    }    // draw player
    window.draw(player.body);

    window.display();
}
