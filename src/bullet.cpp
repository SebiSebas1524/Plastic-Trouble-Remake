#include "bullet.h"
#include "utility.h"

static const int kBulletFrameCount = 5;

Bullet::Bullet()
{
    bulletTexture = LoadTexture("assets/Sprites/Bullet.png");
}

Bullet::Bullet(Bullet&& other) noexcept
    : pos(other.pos)
    , ropeBottom(other.ropeBottom)
    , speed(other.speed)
    , ani(other.ani)
    , active(other.active)
    , bulletTexture(other.bulletTexture)
{
    other.pos          = { -1000.0f, -1000.0f };
    other.ropeBottom   = { -1000.0f, -1000.0f };
    other.active       = false;
    other.bulletTexture = {};
}

Bullet& Bullet::operator=(Bullet&& other) noexcept
{
    if (this != &other)
    {
        UnloadTexture(bulletTexture);

        pos          = other.pos;
        ropeBottom   = other.ropeBottom;
        speed        = other.speed;
        ani          = other.ani;
        active       = other.active;
        bulletTexture = other.bulletTexture;

        other.pos          = { -1000.0f, -1000.0f };
        other.ropeBottom   = { -1000.0f, -1000.0f };
        other.active       = false;
        other.bulletTexture = {};
    }
    return *this;
}

Bullet::~Bullet()
{
    UnloadTexture(bulletTexture);
}

void Bullet::fire(Vector2 origin, float anchorY)
{
    pos               = origin;
    ropeBottom        = origin;
    ropeBottom.y      = anchorY;
    active            = true;
    ani               = 0.0f;
}

void Bullet::cancel()
{
    active     = false;
    pos        = { -1000.0f, -1000.0f };
    ropeBottom = { -1000.0f, -1000.0f };
}

void Bullet::update(float delta)
{
    if (!active) return;

    pos.y -= speed * delta;

    animation();
    drawRope();
    draw();

    // Retract automatically when it reaches the top of the screen
    if (pos.y < -20.0f)
        cancel();
}

void Bullet::draw()
{
    int fw = getWidth();
    int fh = getHeight();
    int frameIndex = (int)ani;

    Rectangle src = { (float)(fw * frameIndex), 0.0f, (float)fw, (float)fh };
    Rectangle dst = {
        (float)ScaleToWindow(pos.x),
        (float)ScaleToWindow(pos.y),
        (float)ScaleToWindow(fw),
        (float)ScaleToWindow(fh)
    };
    DrawTexturePro(bulletTexture, src, dst, { 0, 0 }, 0.0f, WHITE);
}

void Bullet::drawRope() const
{
    // Draw a vertical line from the bullet tip down to where it was fired
    float cx = pos.x + getWidth() / 2.0f;

    Vector2 top    = { (float)ScaleToWindow(cx), (float)ScaleToWindow(pos.y) };
    Vector2 bottom = { (float)ScaleToWindow(cx), (float)ScaleToWindow(ropeBottom.y) };

    DrawLineEx(bottom, top, (float)ScaleToWindow(2), { 180, 220, 255, 200 });
}

void Bullet::animation()
{
    ani += 0.1f;
    if (ani >= kBulletFrameCount) ani = 0.0f;
}

Vector2 Bullet::getMidPos() const
{
    return {
        pos.x + getWidth() / 2.0f,
        pos.y + getHeight() / 5.0f
    };
}

int Bullet::getWidth() const  { return bulletTexture.width / kBulletFrameCount; }
int Bullet::getHeight() const { return bulletTexture.height; }
