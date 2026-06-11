#pragma once
#include "raylib.h"

class Player;

class Bubble
{
public:
    Bubble() = default;
    Bubble(Vector2 pos, int radius, bool dirRight, float incr);
    ~Bubble();

    Bubble(const Bubble&) = delete;
    Bubble& operator=(const Bubble&) = delete;
    Bubble(Bubble&& other) noexcept;
    Bubble& operator=(Bubble&& other) noexcept;

    void load();
    void update(float delta);
    void restart();
    void pop();
    void stop();
    void setPos(float x);
    void setInitialBounce(float upwardVelocity);
    void setInvincible(float duration);

    bool checkPlayerCollision(const Player& player) const;
    bool checkBulletCollision(const Player& player) const;
    bool checkTraceCollision(Vector2 lineStart, Vector2 lineEnd) const;

    bool getActive() const;
    bool isInvincible() const { return invincibleTimer > 0.0f; }
    int getRadius() const;
    float getBounceSpeed() const;
    Vector2 getPos() const;
    Vector2 getCenter() const;

private:
    void drawBubble();
    void bounce(float delta);
    void animation();

    Vector2 pos {};
    Vector2 firstPos {};
    int radius = 30;
    float bounceSpeed = 0.0f;
    float ani = 0.0f;
    float move = 1.0f;
    float gravity = 100.0f;
    float invincibleTimer = 0.0f;

    bool active = true;
    bool inScreen = false;

    Texture2D bubbleTexture {};
    Sound bounceSound {};
};
