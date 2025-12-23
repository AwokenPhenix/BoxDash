#include "Game.hpp"
#include "Config.hpp"
#include <cmath>

static float clampf(float v, float lo, float hi) {
    return std::max(lo, std::min(v, hi));
}

static void drawNeonTri(sf::RenderTarget& target,
    const sf::ConvexShape& base,
    sf::Color neon)
{
    // thin outline glow
    sf::ConvexShape glow = base;
    glow.setFillColor(sf::Color::Transparent);
    glow.setOutlineThickness(2.f);
    glow.setOutlineColor(sf::Color(neon.r, neon.g, neon.b, 120));
    target.draw(glow, sf::BlendAdd);

    // tighter halo
    glow.setOutlineThickness(4.f);
    glow.setOutlineColor(sf::Color(neon.r, neon.g, neon.b, 40));
    target.draw(glow, sf::BlendAdd);

    // solid core
    sf::ConvexShape core = base;
    core.setFillColor(neon);
    core.setOutlineThickness(0.f);
    target.draw(core);
}

static void drawNeonRect(sf::RenderTarget& target,
    const sf::RectangleShape& base,
    sf::Color neon)
{
    // --- thin glow outline ---
    sf::RectangleShape glow = base;
    glow.setFillColor(sf::Color::Transparent);
    glow.setOutlineThickness(2.f);
    glow.setOutlineColor(sf::Color(neon.r, neon.g, neon.b, 120));
    target.draw(glow, sf::BlendAdd);

    // --- extra tight halo (1px) ---
    glow.setOutlineThickness(4.f);
    glow.setOutlineColor(sf::Color(neon.r, neon.g, neon.b, 40));
    target.draw(glow, sf::BlendAdd);

    // --- solid core ---
    sf::RectangleShape core = base;
    core.setFillColor(neon);
    core.setOutlineThickness(0.f);
    target.draw(core);
}


Game::Game()
    : window(sf::VideoMode(sf::Vector2u(960, 540)), "Platformer"),
    view({ 0,0 }, { VIEW_W, VIEW_H })
{
    window.setFramerateLimit(60);
    window.setView(view);

    // Player
    player.body.setSize({ PLAYER_SIZE, PLAYER_SIZE });
    player.body.setFillColor(sf::Color(0, 255, 180));
    player.respawn();

    // Level
    level.loadFromFile("src/Levels/level1.txt");

    // Normal enemies
    for (int i = 0; i < 4; ++i) {
        Enemy e;
        e.type = EnemyType::Walker;   // 

        e.body.setSize({ 28.f, 28.f });
        e.body.setFillColor(sf::Color::Red);
        e.body.setPosition({ 600.f + i * 60.f, GROUND_Y - 28.f });

        enemies.push_back(e);
    }

    // Attack box
    attackBox.setSize({ 24.f, 28.f });
    attackBox.setFillColor(sf::Color(255, 255, 255, 200));

    // Spike factory
    auto makeSpike = [&](float x, float y, float w, float h) {
        Enemy e;
        e.type = EnemyType::Spike;

        e.body.setSize({ w, h });
        e.body.setPosition({ x, y });
        e.body.setFillColor(sf::Color::Transparent);

        e.visual.setPointCount(3);
        e.visual.setPoint(0, { 0.f, h });
        e.visual.setPoint(1, { w / 2.f, 0.f });
        e.visual.setPoint(2, { w, h });
        e.visual.setFillColor(sf::Color::Red);
        e.visual.setPosition({ x, y });

        enemies.push_back(e);
        };

    // Instantiate spikes from level data
    for (auto& s : level.spikes) {
        makeSpike(s.x, s.y, s.w, s.h);
    }
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
            editorMode = !editorMode;
        }

    }

}


// ------------------------------------------------
// Update (TEMP: does nothing yet)
// ------------------------------------------------
void Game::update(float dt) {
    if (editorMode) {
        // Disable gameplay
        player.vel = { 0.f, 0.f };

        // Mouse world position
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

        // Snap to grid
        float gx = std::floor(mouseWorld.x / GRID_SIZE) * GRID_SIZE;
        float gy = std::floor(mouseWorld.y / GRID_SIZE) * GRID_SIZE;

        // Left click ? add platform
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            level.addPlatform(gx, gy, GRID_SIZE * 4.f, GRID_SIZE);
        }

        // Right click ? delete platform
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            for (auto it = level.platforms.begin(); it != level.platforms.end(); ++it) {
                if (it->getGlobalBounds().contains(mouseWorld)) {
                    level.platforms.erase(it);
                    break;
                }
            }
        }

        // Save
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            level.loadFromFile("src/Levels/level1.txt");
        }

        return; // skip normal gameplay update
    }



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

    bool jumpDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    bool jumpJustPressed = jumpDown && !jumpButtonHeld;
    jumpButtonHeld = jumpDown;

    bool dashPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    bool slamPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
    bool attackDown =
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Detect fresh press (edge)
    bool attackJustPressed = attackDown && !attackButtonHeld;
    attackButtonHeld = attackDown;

    if (attackJustPressed && !player.attacking) {
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

    // ---------------- JUMP / DOUBLE JUMP / WALL JUMP ----------------
    if (jumpJustPressed && !player.slamming) {
        if (player.touchingWall && !player.grounded) {
            // wall jump
            player.vel.y = JUMP_VEL;
            player.vel.x = -player.wallDir * WALL_JUMP_X;
            player.touchingWall = false;

            // after a wall jump, you still get 1 air jump
            player.jumpsRemaining = player.maxJumps - 1;
        }
        else if (player.jumpsRemaining > 0) {
            // ground jump or double jump
            player.vel.y = JUMP_VEL;
            player.jumpsRemaining--;
            player.grounded = false;
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
                player.jumpsRemaining = player.maxJumps; 

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
    // ---------------- SPIKE COLLISION ----------------
    sf::FloatRect playerBox = player.body.getGlobalBounds();

    for (auto& e : enemies) {
        if (!e.alive) continue;

        if (e.type == EnemyType::Spike &&
            playerBox.findIntersection(e.body.getGlobalBounds())) {
            player.respawn();
            break;
        }
    }
    // ---------------- NORMAL ATTACK HITBOX ----------------
    if (player.attacking) {
        attackBox.setPosition({
            player.body.getPosition().x + player.facing * PLAYER_SIZE,
            player.body.getPosition().y + 2.f
            });

        for (auto& e : enemies) {
            if (!e.alive) continue;

            // Attacks do NOT affect spikes
            if (e.type == EnemyType::Spike)
                continue;

            if (attackBox.getGlobalBounds().findIntersection(e.body.getGlobalBounds())) {
                e.alive = false;
                player.canDash = true;
                hitstopTimer = HITSTOP_TIME;
                break; // one hit per attack
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
    window.clear(sf::Color(8, 8, 16)); // deep navy-black

    for (auto& p : level.platforms)
        drawNeonRect(window, p, sf::Color(0, 220, 255));

    drawNeonRect(window, player.body, sf::Color(0, 255, 180));

    // draw level
    for (auto& p : level.platforms) {
        window.draw(p);
    }

    // draw enemies
    for (auto& e : enemies) {
        if (!e.alive) continue;

        if (e.type == EnemyType::Spike)
            drawNeonTri(window, e.visual, sf::Color(255, 80, 120));
        else
            drawNeonRect(window, e.body, sf::Color(255, 70, 70));
    }


    if (player.attacking) {
        window.draw(attackBox);
    }    // draw player
    window.draw(player.body);

    window.display();
}
