#include "Level.hpp"

void Level::build() {
    platforms.clear();

    auto makePlatform = [&](float x, float y, float w, float h) {
        sf::RectangleShape p;
        p.setSize({ w, h });
        p.setPosition({ x, y });
        p.setFillColor(sf::Color(0, 200, 255));
        platforms.push_back(p);
        };

    makePlatform(-500.f, 500.f, 3000.f, 48.f);
    makePlatform(300.f, 350.f, 48.f, 260.f);
    makePlatform(550.f, 420.f, 180.f, 48.f);
    makePlatform(850.f, 380.f, 180.f, 48.f);
}
