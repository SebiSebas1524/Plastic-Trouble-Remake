#include "player.h"
#include "utility.h"
#include <utility>
#include <string>

static const int kPlayerFrameCount = 14;

Player::Player(Vector2 pos)
    : pos(pos)
    , firstPos(pos)
{
    playerTexture = LoadTexture("assets/Sprites/playerAnimation.png");
    laserSound    = LoadSound("assets/SFX/laserShoot.wav");
    SetSoundVolume(laserSound, 0.07f);
}

Player::Player(Player&& other) noexcept
    : pos(other.pos)
    , firstPos(other.firstPos)
    , frameIndex(other.frameIndex)
    , frame(other.frame)
    , dir(other.dir)
    , alive(other.alive)
    , actT(other.actT)
    , speed(other.speed)
    , points(other.points)
    , bestScore(other.bestScore)
    , hasPowerUp(other.hasPowerUp)
    , activePowerUp(other.activePowerUp)
    , powerUpTimer(other.powerUpTimer)
    , bullet(std::move(other.bullet))
    , playerTexture(other.playerTexture)
    , laserSound(other.laserSound)
{
    other.playerTexture = {};
    other.laserSound    = {};
}

Player& Player::operator=(Player&& other) noexcept
{
    if (this != &other)
    {
        UnloadTexture(playerTexture);
        UnloadSound(laserSound);

        pos           = other.pos;
        firstPos      = other.firstPos;
        frameIndex    = other.frameIndex;
        frame         = other.frame;
        dir           = other.dir;
        alive         = other.alive;
        actT          = other.actT;
        speed         = other.speed;
        points        = other.points;
        bestScore     = other.bestScore;
        hasPowerUp    = other.hasPowerUp;
        activePowerUp = other.activePowerUp;
        powerUpTimer  = other.powerUpTimer;
        bullet        = std::move(other.bullet);
        playerTexture = other.playerTexture;
        laserSound    = other.laserSound;

        other.playerTexture = {};
        other.laserSound    = {};
    }
    return *this;
}

Player::~Player()
{
    UnloadTexture(playerTexture);
    UnloadSound(laserSound);
}

void Player::update(float delta)
{
    if (!alive) return;

#ifdef _DEBUG
    DrawRectangleLines(ScaleToWindow(pos.x), ScaleToWindow(pos.y + 3),
                       ScaleToWindow(getWidth() - 7), ScaleToWindow(getHeight()), YELLOW);
#endif

    move(delta);
    shoot(delta);
    animation();
    updatePowerUp(delta);

    if (pos.x < 0) pos.x = 0;
    if (pos.x > kVirtualScreenWidth - getWidth())
        pos.x = (float)(kVirtualScreenWidth - getWidth());

    draw();
    drawPowerUpHUD();
}

void Player::draw()
{
    int fw = getWidth();
    int fh = getHeight();
    Rectangle src = { (float)(fw * frameIndex), 0.0f, (float)fw, (float)fh };
    Rectangle dst = {
        (float)ScaleToWindow(pos.x),
        (float)ScaleToWindow(pos.y),
        (float)ScaleToWindow(fw),
        (float)ScaleToWindow(fh)
    };
    DrawTexturePro(playerTexture, src, dst, { 0, 0 }, 0.0f, WHITE);
}

void Player::move(float delta)
{
    bool left  = IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A);
    bool right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

    if (!left && !right) { dir = 'I'; return; }
    if (left  && right)  { dir = 'I'; return; }

    if (right) { pos.x += speed * delta; dir = 'R'; }
    if (left)  { pos.x -= speed * delta; dir = 'L'; }
}

void Player::animation()
{
    if (dir == 'R')
    {
        frame += 0.5f;
        if (frame > 12.0f) frame = 7.0f;
        frameIndex = (int)frame;
    }
    else if (dir == 'L')
    {
        frame -= 0.5f;
        if (frame < 0.0f) frame = 4.0f;
        frameIndex = (int)frame;
    }
    else if (dir == 'I') frameIndex = 6;
    else if (dir == 'S') frameIndex = 13;
}

void Player::shoot(float delta)
{
    int fw = getWidth();
    bool rapidFire = hasPowerUp && activePowerUp == PowerUp::PowerUpType::RAPID_FIRE;

    // Cooldown countdown
    if (shootCooldown > 0.0f)
        shootCooldown -= delta;

    if (IsKeyPressed(KEY_SPACE) && shootCooldown <= 0.0f)
    {
        bullet.cancel();

        PlaySound(laserSound);
        actT = 0.0f;
        shootCooldown = rapidFire ? 0.2f : 0.5f; // shorter cooldown with power-up

        float originX = pos.x + fw / 4.0f - 5;
        float originY = pos.y;
        float feetY   = pos.y + getHeight();
        bullet.fire({ originX, originY }, feetY);
    }

    if (bullet.isFiring())
    {
        if (actT < 5.0f) dir = 'S';
        actT += 0.3f;

        float bulletSpeed = rapidFire ? 2.0f : 1.0f;
        bullet.update(delta * bulletSpeed);
    }
}

void Player::applyPowerUp(PowerUp::PowerUpType type)
{
    hasPowerUp    = true;
    activePowerUp = type;
    powerUpTimer  = kPowerUpDuration;
}

void Player::updatePowerUp(float delta)
{
    if (!hasPowerUp) return;

    powerUpTimer -= delta;
    if (powerUpTimer <= 0.0f)
    {
        hasPowerUp   = false;
        powerUpTimer = 0.0f;
    }
}

void Player::drawPowerUpHUD() const
{
    if (!hasPowerUp) return;

    float ratio = powerUpTimer / kPowerUpDuration;
    int barW = ScaleToWindow(40);
    int barH = ScaleToWindow(4);
    int barX = ScaleToWindow(pos.x);
    int barY = ScaleToWindow(pos.y - 8);
    DrawRectangle(barX, barY, (int)(barW * ratio), barH, ORANGE);
}

void Player::restart()
{
    alive        = true;
    hasPowerUp   = false;
    powerUpTimer = 0.0f;
    actT         = 0.0f;
    shootCooldown = 0.0f;
    bullet.cancel();
    if (points > bestScore) bestScore = points;
    points = 0;
}

int Player::getWidth() const  { return playerTexture.width / kPlayerFrameCount; }
int Player::getHeight() const { return playerTexture.height; }
