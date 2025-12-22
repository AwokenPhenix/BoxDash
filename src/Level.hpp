#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Level {
    std::vector<sf::RectangleShape> platforms;

    void build();
};
