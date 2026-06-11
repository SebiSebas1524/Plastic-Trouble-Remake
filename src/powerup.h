#pragma once
#include "raylib.h"

class PowerUp
{
public:
    enum class PowerUpType { RAPID_FIRE };

    PowerUp() = default;
    PowerUp(Vector2 pos, PowerUpType type);
    ~PowerUp();

    void load();
    void update(float delta);
    bool checkPlayerCollision(Vector2 playerPos, int playerW, int playerH) const;

    void collect();
    bool isActive() const       { return active; }
    PowerUpType getType() const { return type; }

private:
    void draw();
    void applyGravity(float delta);

    Vector2 pos {};
    PowerUpType type = PowerUpType::RAPID_FIRE;
    bool active = false;

    float lifetime = 8.0f;
    float elapsed = 0.0f;
    float bobOffset = 0.0f;
    float gravity = 0.0f;
    bool onGround = false;
};
