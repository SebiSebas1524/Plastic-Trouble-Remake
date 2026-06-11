#include "game.h"
#include "utility.h"
#include <algorithm>
#include <string>
#include <memory>

std::vector<EntityManager::SpawnEntry> Game::buildEasy()
{
    Vector2 L = { -40.0f, 150.0f };
    Vector2 R = { (float)kVirtualScreenWidth + 40.0f, 150.0f };
    std::vector<EntityManager::SpawnEntry> v;
    v.emplace_back(0.0f, Bubble(L, 20, true, 5.0f));
    v.emplace_back(10.0f, Bubble(R, 20, false, 5.0f));
    return v;
}

std::vector<EntityManager::SpawnEntry> Game::buildMedium()
{
    Vector2 L = { -40.0f, 150.0f };
    Vector2 R = { (float)kVirtualScreenWidth + 40.0f, 150.0f };
    std::vector<EntityManager::SpawnEntry> v;
    v.emplace_back(0.0f, Bubble(L, 30, true, 3.0f));
    v.emplace_back(10.0f, Bubble(R, 32, false, 3.0f));
    v.emplace_back(15.0f, Bubble(L, 25, true, 6.0f));
    v.emplace_back(18.0f, Bubble(R, 26, false, 6.0f));
    v.emplace_back(21.0f, Bubble(L, 19, true, 9.0f));
    v.emplace_back(27.0f, Bubble(R, 15, false, 9.0f));
    return v;
}

std::vector<EntityManager::SpawnEntry> Game::buildHard()
{
    Vector2 L = { -40.0f, 150.0f };
    Vector2 R = { (float)kVirtualScreenWidth + 40.0f, 150.0f };
    std::vector<EntityManager::SpawnEntry> v;
    v.emplace_back(0.0f, Bubble(L, 30, true, 2.0f));
    v.emplace_back(10.0f, Bubble(R, 32, false, 2.0f));
    v.emplace_back(15.0f, Bubble(L, 25, true, 3.0f));
    v.emplace_back(18.0f, Bubble(R, 13, false, 3.0f));
    v.emplace_back(21.0f, Bubble(L, 15, true, 9.0f));
    v.emplace_back(27.0f, Bubble(R, 17, false, 9.0f));
    v.emplace_back(35.0f, Bubble(L, 31, true, 5.0f));
    v.emplace_back(40.0f, Bubble(R, 35, false, 5.0f));
    v.emplace_back(42.0f, Bubble(L, 25, true, 10.0f));
    v.emplace_back(47.0f, Bubble(R, 33, false, 10.0f));
    v.emplace_back(49.0f, Bubble(L, 19, true, 7.0f));
    v.emplace_back(50.0f, Bubble(R, 13, false, 7.0f));
    return v;
}

void Game::init()
{
    inMenu = true;
    menuType = Menu::Type::MENU;
    level = EASY;
    timer = 0.0f;

    Vector2 defaultPosPlayer = {
        kVirtualScreenWidth / 2.0f - 25,
        kVirtualScreenHeight - 119.5f
    };

    player = Player(defaultPosPlayer);

    background = LoadTexture("assets/Background/underwater.png");
    bgMusic = LoadMusicStream("assets/SFX/underwater.wav");
    popSound = LoadSound("assets/SFX/pop.wav");

    SetMusicVolume(bgMusic, 0.5f);
    PlayMusicStream(bgMusic);

    auto entries = buildEasy();
    for (auto& e : entries) e.bubble.load();
    entityManager.loadLevel(entries);
}

void Game::shutdown()
{
    UnloadTexture(background);
    UnloadMusicStream(bgMusic);
    UnloadSound(popSound);
}

void Game::tick(float deltaTime)
{
    UpdateMusicStream(bgMusic);

    delta = std::min(deltaTime, 33.333f) / 1000.0f;
    mousePos = GetMousePosition();

    if (inMenu)
    {
        menu.update(menuType, mousePos, inMenu);

        if (menuType != Menu::Type::PAUSE && menuType != Menu::Type::PLAYS)
            restartGame();

        if (menuType == Menu::Type::MENU)
        {
            DrawText("Best score:", ScaleToWindow(kVirtualScreenWidth - kVirtualScreenWidth / 3.0f + 5.0f),
                     ScaleToWindow(5.0f), ScaleToWindow(20.0f), WHITE);
            DrawText(TextFormat("%d", player.getBestScore()),
                     ScaleToWindow(kVirtualScreenWidth - kVirtualScreenWidth / 6.0f),
                     ScaleToWindow(5.0f), ScaleToWindow(20.0f), WHITE);
        }
    }
    else
    {
        updateGame();
    }
}

void Game::updateGame()
{
    Rectangle src = { 0, 0, (float)background.width, (float)background.height };
    Rectangle dst = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    DrawTexturePro(background, src, dst, { 0, 0 }, 0.0f, WHITE);

    player.update(delta);

    menu.update(menuType, mousePos, inMenu);

    if (IsKeyPressed(KEY_R))
        restartGame();

    updateLevel();
    drawHUD();
}

void Game::updateLevel()
{
    timer += delta;

    bool playerHit = false;
    Vector2 dropAt = { -1000.0f, -1000.0f };
    entityManager.update(delta, timer, player, popSound, playerHit, dropAt);

    if (dropAt.x > 0.0f && currentPowerUp == nullptr)
    {
        currentPowerUp = std::make_unique<PowerUp>(dropAt, PowerUp::PowerUpType::RAPID_FIRE);
        currentPowerUp->load();
    }

    if (currentPowerUp != nullptr)
    {
        currentPowerUp->update(delta);

        if (currentPowerUp->checkPlayerCollision(player.getPos(),
                                                  player.getWidth(),
                                                  player.getHeight()))
        {
            player.applyPowerUp(currentPowerUp->getType());
            currentPowerUp.reset();
        }
        else if (!currentPowerUp->isActive())
        {
            currentPowerUp.reset();
        }
    }

    if (playerHit)
    {
        player.restart();
        restartGame();
        menuType = Menu::Type::DEAD;
        inMenu = true;
        return;
    }

    if (entityManager.allInactive())
        advanceLevel();
}

void Game::advanceLevel()
{
    timer = 0.0f;
    currentPowerUp.reset();

    if (level == EASY)
    {
        level = MEDIUM;
        auto entries = buildMedium();
        for (auto& e : entries) e.bubble.load();
        entityManager.loadLevel(entries);
    }
    else if (level == MEDIUM)
    {
        level = HARD;
        auto entries = buildHard();
        for (auto& e : entries) e.bubble.load();
        entityManager.loadLevel(entries);
    }
    else
    {
        menuType = Menu::Type::WIN;
        inMenu = true;
        restartGame();
    }
}

void Game::restartGame()
{
    timer = 0.0f;
    level = EASY;
    currentPowerUp.reset();

    player.setPos({
        kVirtualScreenWidth / 2.0f - 25,
        kVirtualScreenHeight - 119.5f
    });
    player.restart();

    auto entries = buildEasy();
    for (auto& e : entries) e.bubble.load();
    entityManager.loadLevel(entries);
}

void Game::drawHUD()
{
    int levelX = ScaleToWindow(kVirtualScreenWidth - kVirtualScreenWidth / 4.0f);
    int levelValueX = ScaleToWindow(kVirtualScreenWidth - kVirtualScreenWidth / 6.0f);
    int topY = ScaleToWindow(5.0f);
    int bottomY = GetScreenHeight() - ScaleToWindow(25.0f);
    int fontSize = ScaleToWindow(20.0f);

    const char* levelStr = (level == EASY) ? "EASY" : (level == MEDIUM) ? "MEDIUM" : "HARD";
    Color levelColor = (level == EASY) ? GREEN : (level == MEDIUM) ? YELLOW : RED;

    DrawText("Level:", levelX, bottomY, fontSize, WHITE);
    DrawText(levelStr, levelValueX, bottomY, fontSize, levelColor);

    DrawText("Score:", levelX, topY, fontSize, WHITE);
    DrawText(TextFormat("%d", player.getPoints()), levelValueX, topY, fontSize, WHITE);
}
