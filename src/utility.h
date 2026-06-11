#pragma once

// Virtual game resolution
constexpr int kVirtualScreenWidth = 800;
constexpr int kVirtualScreenHeight = 600;

// No more ScaleToWindow — Camera2D handles this automatically
// All game code works in virtual coordinates (0-800, 0-600)
// Raylib scales and letterboxes automatically
