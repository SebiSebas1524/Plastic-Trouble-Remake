#include "raylib.h"
#include "game.h"

int main()
{
    constexpr int screenWidth = 1200;
    constexpr int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Bubble Trouble");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(400, 300);
    SetWindowState(FLAG_WINDOW_HIGHDPI);
    InitAudioDevice();
    SetTargetFPS(60);

    Game game;
    game.init();

    while (!WindowShouldClose())
    {
        // Enforce 9:5 aspect ratio (900:500)
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        int targetH = (w * 5) / 9;
        if (h != targetH)
        {
            SetWindowSize(w, targetH);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        game.tick(GetFrameTime() * 1000.0f);
        EndDrawing();
    }

    game.shutdown();
    CloseAudioDevice();
    CloseWindow();
}
