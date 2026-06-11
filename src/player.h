#pragma once
#include "raylib.h"
#include "bullet.h"
#include "powerup.h"

class Player
{
public:
    Player() = default;
    Player(Vector2 pos);
    ~Player();

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&& other) noexcept;
    Player& operator=(Player&& other) noexcept;

    void update(float delta);
    void restart();

    void applyPowerUp(PowerUp::PowerUpType type);

    void setPos(Vector2 newPos) { pos = newPos; }
    void setPoints(int point)   { points += point; }

    Vector2 getPos() const      { return pos; }
    int getWidth() const;
    int getHeight() const;
    int getPoints() const       { return points; }
    int getBestScore() const    { return bestScore; }

    // Bullet/rope getters used by collision
    Vector2 getBulletPos() const       { return bullet.getRopeTop(); }
    Vector2 getBulletMidPos() const    { return bullet.getMidPos(); }
    Vector2 getBulletTraceTail() const { return bullet.getRopeBottom(); }
    int getBulletWidth() const         { return bullet.getWidth(); }
    int getBulletHeight() const        { return bullet.getHeight(); }
    bool isFiring() const              { return bullet.isFiring(); }

private:
    void draw();
    void move(float delta);
    void animation();
    void shoot(float delta);
    void updatePowerUp(float delta);
    void drawPowerUpHUD() const;

    Vector2 pos {};
    Vector2 firstPos {};

    int frameIndex  = 6;
    float frame     = 6.0f;
    char dir        = 'I';

    bool alive      = true;
    float actT      = 0.0f;
    float shootCooldown = 0.0f;
    float speed     = 300.0f;

    int points      = 0;
    int bestScore   = 0;

    // Power-up state
    bool hasPowerUp = false;
    PowerUp::PowerUpType activePowerUp = PowerUp::PowerUpType::RAPID_FIRE;
    float powerUpTimer = 0.0f;
    static constexpr float kPowerUpDuration = 6.0f;

    Bullet bullet;

    Texture2D playerTexture {};
    Sound laserSound {};
};
