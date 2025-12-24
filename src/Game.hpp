#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Level.hpp"


class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::View view;

    Player player;
    Level level;

    std::vector<Enemy> enemies;
    sf::RectangleShape goalShape;
    sf::RectangleShape attackBox;

    int currentLevel = 0;
    std::vector<std::string> levelPaths;


    float hitstopTimer = 0.f;
    bool attackButtonHeld = false;
    bool editorMode = false;
    const float GRID_SIZE = 32.f;
    bool jumpButtonHeld = false;

    void processInput(float dt);
    void update(float dt);
    void render();
    void loadLevel(int index);
    void loadNextLevel();

};
