#pragma once
#include "raylib.h"
#include "entity_manager.h"
#include "player.h"
#include "powerup.h"
#include "menu.h"
#include <vector>
#include <memory>

class Game
{
public:
    public:
    void init();
    void tick(float deltaTime);
    void shutdown();
    void mouseMove(int x, int y) { mousePos = { (float)x, (float)y }; }

private:
    enum LevelType { EASY = 0, MEDIUM, HARD };

    void updateGame();
    void updateLevel();
    void advanceLevel();
    void restartGame();
    void drawHUD();

    std::vector<EntityManager::SpawnEntry> buildEasy();
    std::vector<EntityManager::SpawnEntry> buildMedium();
    std::vector<EntityManager::SpawnEntry> buildHard();

    LevelType level = EASY;
    float timer = 0.0f;
    float delta = 0.0f;
    bool inMenu = true;

    Vector2 mousePos {};

    Menu::Type menuType = Menu::Type::MENU;
    Menu menu;

    EntityManager entityManager;
    Player player;

    // unique_ptr instead of optional — avoids C++17 requirement
    std::unique_ptr<PowerUp> currentPowerUp;

    Texture2D background {};
    Music bgMusic {};
    Sound popSound {};
};
