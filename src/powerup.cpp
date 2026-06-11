#include "powerup.h"
#include "utility.h"
#include <cmath>

static constexpr int kPowerUpSize = 40; // bigger — doubled from 20

PowerUp::PowerUp(Vector2 pos, PowerUpType type)
    : pos(pos)
    , type(type)
    , active(true)
{
}

PowerUp::~PowerUp() {}

void PowerUp::load()
{
    // Add asset
}

void PowerUp::update(float delta)
{
    if (!active) return;

    elapsed += delta;

    applyGravity(delta);

    if (onGround)
        bobOffset = sinf(elapsed * 3.0f) * 4.0f;

    if (elapsed >= lifetime)
        active = false;

    draw();
}

void PowerUp::applyGravity(float delta)
{
    if (onGround) return;

    gravity += 400.0f * delta; // slow fall
    pos.y += gravity * delta;

    int floor = kVirtualScreenHeight - 45 - kPowerUpSize;
    if (pos.y >= (float)floor)
    {
        pos.y = (float)floor;
        gravity = 0.0f;
        onGround = true;
    }
}

void PowerUp::draw()
{
    if (!active) return;

    float cx = pos.x + kPowerUpSize / 2.0f;
    float cy = pos.y + bobOffset + kPowerUpSize / 2.0f;

    // Outer glow
    DrawCircle(ScaleToWindow(cx), ScaleToWindow(cy),
               (float)ScaleToWindow(kPowerUpSize / 2.0f + 4), Fade(GOLD, 0.31f));

    // Main body
    DrawCircle(ScaleToWindow(cx), ScaleToWindow(cy),
               (float)ScaleToWindow(kPowerUpSize / 2.0f), Fade(ORANGE, 0.94f));

    // Label
    int fontSize = ScaleToWindow(14);
    int textW = MeasureText("2x", fontSize);
    int textX = ScaleToWindow(cx) - textW / 2;
    int textY = ScaleToWindow(cy) - fontSize / 2;
    DrawText("2x", textX, textY, fontSize, WHITE);

    // Timer bar
    float ratio = 1.0f - (elapsed / lifetime);
    int barW = ScaleToWindow(kPowerUpSize);
    int barH = ScaleToWindow(4);
    int barX = ScaleToWindow(pos.x);
    int barY = ScaleToWindow(pos.y + bobOffset + kPowerUpSize + 3);
    DrawRectangle(barX, barY, (int)(barW * ratio), barH, YELLOW);
}

bool PowerUp::checkPlayerCollision(Vector2 playerPos, int playerW, int playerH) const
{
    if (!active) return false;

    Rectangle playerRect = { playerPos.x, playerPos.y, (float)playerW, (float)playerH };
    Rectangle powerRect = { pos.x, pos.y, (float)kPowerUpSize, (float)kPowerUpSize };
    return CheckCollisionRecs(playerRect, powerRect);
}

void PowerUp::collect()
{
    active = false;
}
