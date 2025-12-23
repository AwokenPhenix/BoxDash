#pragma once
#include <SFML/Graphics.hpp>

enum class EnemyType {
    Walker,
    Spike
};

struct Enemy {
    EnemyType type = EnemyType::Walker; // 

    sf::RectangleShape body;
    sf::ConvexShape visual;

    bool alive = true;
};
