#pragma once
#include <raylib.h>

class Ball
{
public:
    Ball();
    void Update();
    void Draw() const;

private:
    int x;
    int y;
    int speedX;
    int speedY;
    int radius;
    Texture2D texture;
};