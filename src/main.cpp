#include "raylib.h"
#include "game.h"
#include "utility.h"

int main()
{
    // Window can be any size — camera handles scaling
    InitWindow(1280, 720, "Bubble Trouble");
    InitAudioDevice();
    SetTargetFPS(60);

    // Camera setup for automatic scaling/letterboxing
    Camera2D camera = { 0 };
    camera.target = { 0.0f, 0.0f };
    camera.offset = { 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Game game;
    game.init();

    while (!WindowShouldClose())
    {
        // Calculate camera zoom to fit virtual resolution
        float scale = fminf(
            (float)GetScreenWidth() / kVirtualScreenWidth,
            (float)GetScreenHeight() / kVirtualScreenHeight
        );
        camera.zoom = scale;
        
        // Center camera offset for letterboxing
        camera.offset.x = (GetScreenWidth() - (kVirtualScreenWidth * scale)) / 2.0f;
        camera.offset.y = (GetScreenHeight() - (kVirtualScreenHeight * scale)) / 2.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        
        BeginMode2D(camera);
        game.tick(GetFrameTime() * 1000.0f);
        EndMode2D();
        
        EndDrawing();
    }

    game.shutdown();
    CloseAudioDevice();
    CloseWindow();
}
