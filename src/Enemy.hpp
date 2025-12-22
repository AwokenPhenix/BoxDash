#pragma once
#include <SFML/Graphics.hpp>

struct Enemy {
    sf::RectangleShape body;
    bool alive = true;
};
