#include "Level.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
// ---------------- CLEAR ----------------
void Level::clear() {
    platforms.clear();
    spikes.clear();
    goals.clear();
}

// ---------------- ADD PLATFORM ----------------
void Level::addPlatform(float x, float y, float w, float h) {
    sf::RectangleShape p;
    p.setSize({ w, h });
    p.setPosition({ x, y });
    p.setFillColor(sf::Color(0, 200, 255));
    platforms.push_back(p);
}

// ---------------- LOAD ----------------
bool Level::loadFromFile(const std::string& path) {
    clear();

    std::ifstream file(path);
    if (!file.is_open())
        return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "PLATFORM") {
            float x, y, w, h;
            ss >> x >> y >> w >> h;
            addPlatform(x, y, w, h);
        }
        else if (type == "SPIKE") {
            SpikeData s;
            ss >> s.x >> s.y >> s.w >> s.h;
            spikes.push_back(s);
        }
        else if (type == "GOAL") {
            GoalData g;
            ss >> g.x >> g.y >> g.size;
            goals.push_back(g);
            std::cout << "Loaded GOAL at " << g.x << ", " << g.y << "\n";


        }
       

    }

    return true;
}

// ---------------- SAVE ----------------
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

    for (auto& s : spikes) {
        file << "SPIKE "
            << s.x << " "
            << s.y << " "
            << s.w << " "
            << s.h << "\n";
    }

    return true;
}
