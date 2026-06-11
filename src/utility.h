#pragma once
#include "raylib.h"

constexpr int kVirtualScreenWidth = 900;
constexpr int kVirtualScreenHeight = 500;

inline float GetWindowScale()
{
    float scaleX = (float)GetScreenWidth() / kVirtualScreenWidth;
    float scaleY = (float)GetScreenHeight() / kVirtualScreenHeight;
    return (scaleX < scaleY) ? scaleX : scaleY;
}

inline int ScaleToWindow(float virtualCoord)
{
    return (int)(virtualCoord * GetWindowScale());
}
