#include "Level.hpp"
#include <fstream>
#include <sstream>

void Level::clear() {
    platforms.clear();
}

void Level::addPlatform(float x, float y, float w, float h) {
    sf::RectangleShape p;
    p.setSize({ w, h });
    p.setPosition({ x, y });
    p.setFillColor(sf::Color(0, 200, 255));
    platforms.push_back(p);
}

bool Level::loadFromFile(const std::string& path) {
    clear();

    std::ifstream file(path);
    if (!file.is_open())
        return false;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "PLATFORM") {
            float x, y, w, h;
            ss >> x >> y >> w >> h;
            addPlatform(x, y, w, h);
        }
    }

    return true;
}

bool Level::saveToFile(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open())
        return false;

    for (auto& p : platforms) {
        auto pos = p.getPosition();
        auto size = p.getSize();
        file << "PLATFORM "
            << pos.x << " "
            << pos.y << " "
            << size.x << " "
            << size.y << "\n";
    }

    return true;
}
