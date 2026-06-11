#pragma once
#include "raylib.h"
#include <functional>
#include <vector>

class Menu
{
public:
    enum class Type { MENU, PLAYS, PAUSE, CONTROLS, DEAD, WIN };

    Menu();
    ~Menu();

    void update(Type& menuType, Vector2 mousePos, bool& inMenu);

private:
    // A button is a pair of textures (normal + hover) at a virtual position,
    // with an action to call when clicked.
    struct Button
    {
        Texture2D normal {};
        Texture2D hover {};
        float vx = 0.0f; // virtual x (pre-scaling)
        float vy = 0.0f; // virtual y (pre-scaling)
        std::function<void()> onClick;
    };

    void processButtons(std::vector<Button>& buttons, Vector2 mousePos);

    void updatePrincipalMenu(Type& menuType, Vector2 mousePos, bool& inMenu);
    void updatePlays(Type& menuType, Vector2 mousePos, bool& inMenu);
    void updatePause(Type& menuType, Vector2 mousePos, bool& inMenu);
    void updateControls(Type& menuType, Vector2 mousePos, bool& inMenu);
    void updateDead(Type& menuType, Vector2 mousePos, bool& inMenu);
    void updateWin(Type& menuType, Vector2 mousePos, bool& inMenu);

    bool played = false;

    Texture2D menuBackground {};
    Texture2D controlMenu {};
    Texture2D deadMenu {};
    Texture2D pauseMenu {};
    Texture2D winMenu {};

    Texture2D startButton1 {};
    Texture2D startButton2 {};
    Texture2D exitButton1 {};
    Texture2D exitButton2 {};
    Texture2D backButton1 {};
    Texture2D backButton2 {};
    Texture2D controlButton1 {};
    Texture2D controlButton2 {};
    Texture2D replayButton1 {};
    Texture2D replayButton2 {};
    Texture2D menuButton1 {};
    Texture2D menuButton2 {};
    Texture2D resumeButton1 {};
    Texture2D resumeButton2 {};

    Sound clickSound {};
    Sound winSound {};
    Sound loseSound {};
};
