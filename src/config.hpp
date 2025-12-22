#pragma once
#include <SFML/Graphics.hpp>

// Physics
constexpr float GRAVITY = 3000.f;
constexpr float MOVE_SPEED = 450.f;
constexpr float JUMP_VEL = -900.f;

constexpr float WALL_JUMP_X = 650.f;
constexpr float WALL_SLIDE_SPEED = 300.f;

constexpr float DASH_SPEED = 1200.f;
constexpr float DASH_TIME = 0.12f;

constexpr float ATTACK_TIME = 0.12f;

// World
constexpr float VIEW_W = 960.f;
constexpr float VIEW_H = 540.f;

constexpr float GROUND_Y = 500.f;
constexpr float PLAYER_SIZE = 32.f;

constexpr float FALL_DEATH_Y = 900.f;
const sf::Vector2f SPAWN_POINT{ 100.f, GROUND_Y - PLAYER_SIZE };
constexpr float SLAM_SPEED = 2200.f;
constexpr float HITSTOP_TIME = 0.06f;
