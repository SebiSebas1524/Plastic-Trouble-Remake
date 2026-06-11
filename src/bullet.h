#pragma once
#include "raylib.h"

class Bullet
{
public:
    Bullet();
    ~Bullet();

    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
    Bullet(Bullet&& other) noexcept;
    Bullet& operator=(Bullet&& other) noexcept;

    void fire(Vector2 origin, float anchorY);
    void cancel();               // retract the bullet immediately
    void update(float delta);

    Vector2 getRopeTop() const { return pos; }         // bullet tip (moving up)
    Vector2 getRopeBottom() const { return ropeBottom; } // fixed at player feet

    Vector2 getMidPos() const;
    int getWidth() const;
    int getHeight() const;

    bool isFiring() const { return active; }
    float getPosY() const { return pos.y; }

private:
    void draw();
    void drawRope() const;
    void animation();

    Vector2 pos = { -1000.0f, -1000.0f };
    Vector2 ropeBottom = { -1000.0f, -1000.0f };
    float speed = 250.0f;
    float ani = 0.0f;
    bool active = false;

    Texture2D bulletTexture {};
};
