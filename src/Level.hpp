#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Level {
    std::vector<sf::RectangleShape> platforms;

    void clear();
    void addPlatform(float x, float y, float w, float h);

    bool loadFromFile(const std::string& path);
    bool saveToFile(const std::string& path);
};
