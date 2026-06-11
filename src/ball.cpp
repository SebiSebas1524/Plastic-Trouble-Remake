#include "ball.h"
#include "utility.h"

Ball::Ball()
    : x(100)
    , y(100)
    , speedX(5)
    , speedY(5)
    , radius(15)
{
    texture = LoadTexture("assets/Sprites/bubble.png");
}

void Ball::Update()
{
    x += speedX;
    y += speedY;

    const int screenWidth = kVirtualScreenWidth;
    const int screenHeight = kVirtualScreenHeight;

    if ((x + radius >= screenWidth) || (x - radius <= 0)) {
        speedX *= -1;
    }

    if ((y + radius >= screenHeight) || (y - radius <= 0)) {
        speedY *= -1;
    }
}

void Ball::Draw() const
{
    unsigned numFrames = 8;
	int frameWidth = texture.width / numFrames;
	Rectangle frameRec = { (float)frameWidth * 6, 0.0f, (float)frameWidth, (float)texture.height };

    Vector2 pos = { (float)ScaleToWindow((float)x), (float)ScaleToWindow((float)y) };

    DrawTextureRec(texture, frameRec, pos, WHITE);
}