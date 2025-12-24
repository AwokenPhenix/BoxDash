#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// --------- DATA TYPES ---------
struct SpikeData {
    float x;
    float y;
    float w;
    float h;
};


struct GoalData {
    float x;
    float y;
    float size;
};

// --------- LEVEL ---------
struct Level {
    std::vector<sf::RectangleShape> platforms;
    std::vector<SpikeData> spikes;
    std::vector<GoalData> goals;
    void clear();

    void addPlatform(float x, float y, float w, float h);

    bool loadFromFile(const std::string& path);
    bool saveToFile(const std::string& path);


};
