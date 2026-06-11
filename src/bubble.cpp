#include "bubble.h"
#include "player.h"
#include "utility.h"

Bubble::Bubble(Vector2 pos, int radius, bool dirRight, float incr)
    : pos(pos)
    , firstPos(pos)
    , radius(radius)
    , bounceSpeed(incr)
    , move(dirRight ? 150.0f : -150.0f)
{
}

Bubble::Bubble(Bubble&& other) noexcept
    : pos(other.pos)
    , firstPos(other.firstPos)
    , radius(other.radius)
    , bounceSpeed(other.bounceSpeed)
    , ani(other.ani)
    , move(other.move)
    , gravity(other.gravity)
    , invincibleTimer(other.invincibleTimer)
    , active(other.active)
    , inScreen(other.inScreen)
    , bubbleTexture(other.bubbleTexture)
    , bounceSound(other.bounceSound)
{
    other.bubbleTexture = {};
    other.bounceSound = {};
    other.active = false;
}

Bubble& Bubble::operator=(Bubble&& other) noexcept
{
    if (this != &other)
    {
        UnloadTexture(bubbleTexture);
        UnloadSound(bounceSound);

        pos = other.pos;
        firstPos = other.firstPos;
        radius = other.radius;
        bounceSpeed = other.bounceSpeed;
        ani = other.ani;
        move = other.move;
        gravity = other.gravity;
        invincibleTimer = other.invincibleTimer;
        active = other.active;
        inScreen = other.inScreen;
        bubbleTexture = other.bubbleTexture;
        bounceSound = other.bounceSound;

        other.bubbleTexture = {};
        other.bounceSound = {};
        other.active = false;
    }
    return *this;
}

Bubble::~Bubble()
{
    UnloadTexture(bubbleTexture);
    UnloadSound(bounceSound);
}

void Bubble::load()
{
    bubbleTexture = LoadTexture("assets/Sprites/bubble.png");
    bounceSound   = LoadSound("assets/SFX/bounce.wav");
    SetSoundVolume(bounceSound, 0.07f);
}

void Bubble::setInvincible(float duration)
{
    invincibleTimer = duration;
}

void Bubble::drawBubble()
{
    int frameWidth = bubbleTexture.width / 8;
    int frameIndex = (int)ani;

    Rectangle frameRec = {
        (float)(frameWidth * frameIndex),
        0.0f,
        (float)frameWidth,
        (float)bubbleTexture.height
    };

    Rectangle destRec = {
        (float)ScaleToWindow(pos.x),
        (float)ScaleToWindow(pos.y),
        (float)ScaleToWindow(radius * 2),
        (float)ScaleToWindow(radius * 2)
    };

    // Flash white while invincible by alternating tint every 4 frames
    Color tint = WHITE;
    if (invincibleTimer > 0.0f)
    {
        int flash = (int)(invincibleTimer * 10.0f) % 2;
        tint = flash ? Color{255, 255, 255, 120} : Color{255, 220, 80, 200};
    }

    DrawTexturePro(bubbleTexture, frameRec, destRec, { 0, 0 }, 0.0f, tint);
}

void Bubble::update(float delta)
{
#ifdef _DEBUG
    DrawRectangleLines(ScaleToWindow(pos.x), ScaleToWindow(pos.y),
                       ScaleToWindow(radius * 2), ScaleToWindow(radius * 2), YELLOW);
#endif
    if (!active) return;

    if (invincibleTimer > 0.0f)
        invincibleTimer -= delta;

    if (pos.x >= 0 && pos.y >= 0
        && pos.x <= kVirtualScreenWidth - radius * 2
        && pos.y <= kVirtualScreenHeight)
        inScreen = true;

    drawBubble();
    bounce(delta);
    animation();
}

void Bubble::bounce(float delta)
{
    pos.x += move * delta;

    if (!inScreen) return;

    pos.y += gravity * delta;
    gravity += bounceSpeed;

    int floor = kVirtualScreenHeight - 45 - radius * 2;
    if (pos.y >= (float)floor)
    {
        PlaySound(bounceSound);
        pos.y = (float)floor;
        gravity = -1.0f * (gravity - bounceSpeed);
    }

    if (pos.x > (kVirtualScreenWidth - radius * 2 + 5) || pos.x < -5)
        move = -move;
}

void Bubble::animation()
{
    if (radius >= 20)
    {
        ani += 0.01f;
        if (ani > 3.99f) ani = 0.0f;
    }
    else
    {
        ani += 0.01f;
        if (ani > 7.99f) ani = 4.0f;
    }
}

void Bubble::restart()
{
    active = true;
    gravity = 100.0f;
    pos = firstPos;
    inScreen = false;
    invincibleTimer = 0.0f;
}

void Bubble::pop()
{
    int frameWidth = bubbleTexture.width / 8;
    int frameIndex = (int)ani;

    Rectangle frameRec = {
        (float)(frameWidth * frameIndex),
        0.0f,
        (float)frameWidth,
        (float)bubbleTexture.height
    };

    Rectangle destRec = {
        (float)ScaleToWindow(pos.x),
        (float)ScaleToWindow(pos.y),
        (float)ScaleToWindow(radius * 3),
        (float)ScaleToWindow(radius * 3)
    };

    DrawTexturePro(bubbleTexture, frameRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

void Bubble::stop()          { active = false; }
void Bubble::setPos(float x) { pos.x = x; }

Vector2 Bubble::getCenter() const
{
    return { pos.x + radius, pos.y + radius };
}

bool Bubble::checkPlayerCollision(const Player& player) const
{
    if (invincibleTimer > 0.0f) return false;

    Rectangle playerRect = {
        player.getPos().x,
        player.getPos().y,
        (float)player.getWidth(),
        (float)player.getHeight()
    };
    return CheckCollisionCircleRec(getCenter(), (float)radius, playerRect);
}

bool Bubble::checkBulletCollision(const Player& player) const
{
    if (invincibleTimer > 0.0f) return false;

    Rectangle bulletRect = {
        player.getBulletPos().x,
        player.getBulletPos().y,
        (float)player.getBulletWidth(),
        (float)(player.getBulletHeight() - 25)
    };
    return CheckCollisionCircleRec(getCenter(), (float)radius, bulletRect)
        || CheckCollisionPointCircle(player.getBulletMidPos(), getCenter(), (float)radius);
}

bool Bubble::getActive() const       { return active; }
int Bubble::getRadius() const        { return radius; }
float Bubble::getBounceSpeed() const { return bounceSpeed; }
Vector2 Bubble::getPos() const       { return pos; }

// Exposed for trace collision — checks if a line segment intersects the bubble circle
bool Bubble::checkTraceCollision(Vector2 lineStart, Vector2 lineEnd) const
{
    if (invincibleTimer > 0.0f) return false;
    return CheckCollisionCircleLine(getCenter(), (float)radius, lineStart, lineEnd);
}

void Bubble::setInitialBounce(float upwardVelocity)
{
    gravity = upwardVelocity;
}
