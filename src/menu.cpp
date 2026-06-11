#include "menu.h"
#include "utility.h"

namespace
{
    void DrawTextureFullscreen(Texture2D texture)
    {
        Rectangle src = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        Rectangle dst = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
        DrawTexturePro(texture, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    void DrawScaledTexture(Texture2D texture, float vx, float vy)
    {
        float scale = GetWindowScale();
        Rectangle src = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        Rectangle dst = { vx * scale, vy * scale, texture.width * scale, texture.height * scale };
        DrawTexturePro(texture, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    Rectangle ScaledRect(Texture2D texture, float vx, float vy)
    {
        float scale = GetWindowScale();
        return {
            vx * scale,
            vy * scale,
            texture.width * scale,
            texture.height * scale
        };
    }

    bool IsHovered(Texture2D texture, float vx, float vy, Vector2 mousePos)
    {
        Rectangle r = ScaledRect(texture, vx, vy);
        return CheckCollisionPointRec(mousePos, r);
    }

    float RightVX(Texture2D texture, float margin = 5.0f)
    {
        return kVirtualScreenWidth - texture.width - margin;
    }

    float BottomVY(Texture2D texture, float margin = 5.0f)
    {
        return kVirtualScreenHeight - texture.height - margin;
    }
}

Menu::Menu()
{
    menuBackground = LoadTexture("assets/Background/underwaterMenu.png");
    controlMenu    = LoadTexture("assets/Background/ControlsMenu.png");
    deadMenu       = LoadTexture("assets/Background/DeadMenu.png");
    pauseMenu      = LoadTexture("assets/Background/pauseMenu.png");
    winMenu        = LoadTexture("assets/Background/WinMenu.png");

    startButton1   = LoadTexture("assets/Buttons/START.png");
    startButton2   = LoadTexture("assets/Buttons/START2.png");
    exitButton1    = LoadTexture("assets/Buttons/EXIT.png");
    exitButton2    = LoadTexture("assets/Buttons/EXIT2.png");
    backButton1    = LoadTexture("assets/Buttons/BackB.png");
    backButton2    = LoadTexture("assets/Buttons/BackB2.png");
    controlButton1 = LoadTexture("assets/Buttons/ControlsB.png");
    controlButton2 = LoadTexture("assets/Buttons/ControlsB2.png");
    replayButton1  = LoadTexture("assets/Buttons/ReplayButton.png");
    replayButton2  = LoadTexture("assets/Buttons/ReplayButton2.png");
    menuButton1    = LoadTexture("assets/Buttons/ButtoneMenu.png");
    menuButton2    = LoadTexture("assets/Buttons/ButtonMenu.png");
    resumeButton1  = LoadTexture("assets/Buttons/ResumeButton.png");
    resumeButton2  = LoadTexture("assets/Buttons/ResumeButton2.png");

    clickSound = LoadSound("assets/SFX/click.wav");
    winSound   = LoadSound("assets/SFX/windoot.wav");
    loseSound  = LoadSound("assets/SFX/loseS.wav");
}

Menu::~Menu()
{
    UnloadTexture(menuBackground);
    UnloadTexture(controlMenu);
    UnloadTexture(deadMenu);
    UnloadTexture(pauseMenu);
    UnloadTexture(winMenu);
    UnloadTexture(startButton1);
    UnloadTexture(startButton2);
    UnloadTexture(exitButton1);
    UnloadTexture(exitButton2);
    UnloadTexture(backButton1);
    UnloadTexture(backButton2);
    UnloadTexture(controlButton1);
    UnloadTexture(controlButton2);
    UnloadTexture(replayButton1);
    UnloadTexture(replayButton2);
    UnloadTexture(menuButton1);
    UnloadTexture(menuButton2);
    UnloadTexture(resumeButton1);
    UnloadTexture(resumeButton2);
    UnloadSound(clickSound);
    UnloadSound(winSound);
    UnloadSound(loseSound);
}

void Menu::processButtons(std::vector<Button>& buttons, Vector2 mousePos)
{
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    for (auto& btn : buttons)
    {
        bool hovered = IsHovered(btn.normal, btn.vx, btn.vy, mousePos);
        DrawScaledTexture(hovered ? btn.hover : btn.normal, btn.vx, btn.vy);
        if (hovered && clicked && btn.onClick)
            btn.onClick();
    }
}

void Menu::update(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    switch (menuType)
    {
    case Type::MENU:     updatePrincipalMenu(menuType, mousePos, inMenu); break;
    case Type::PLAYS:    updatePlays(menuType, mousePos, inMenu);         break;
    case Type::PAUSE:    updatePause(menuType, mousePos, inMenu);         break;
    case Type::CONTROLS: updateControls(menuType, mousePos, inMenu);      break;
    case Type::DEAD:     updateDead(menuType, mousePos, inMenu);          break;
    case Type::WIN:      updateWin(menuType, mousePos, inMenu);           break;
    default: break;
    }
}

void Menu::updatePrincipalMenu(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    DrawTextureFullscreen(menuBackground);

    std::vector<Button> buttons = {
        { startButton1, startButton2, 360.0f, 220.0f,
          [&]{ PlaySound(clickSound); menuType = Type::PLAYS; inMenu = true; } },

        { controlButton1, controlButton2, 360.0f, 300.0f,
          [&]{ PlaySound(clickSound); menuType = Type::CONTROLS; inMenu = true; } },

        { exitButton1, exitButton2, 360.0f, 380.0f,
          [&]{ PlaySound(clickSound); CloseWindow(); } },
    };

    processButtons(buttons, mousePos);
}

void Menu::updatePlays(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    inMenu = false;

    float backVY = BottomVY(backButton1);
    std::vector<Button> buttons = {
        { backButton1, backButton2, 10.0f, backVY,
          [&]{ PlaySound(clickSound); menuType = Type::MENU; inMenu = true; } },
    };

    processButtons(buttons, mousePos);

    if (IsKeyPressed(KEY_ESCAPE))
    {
        menuType = Type::PAUSE;
        inMenu = true;
    }
}

void Menu::updatePause(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    DrawTextureFullscreen(pauseMenu);

    float menuVX = RightVX(menuButton1);
    float menuVY = BottomVY(menuButton1);

    std::vector<Button> buttons = {
        { menuButton1, menuButton2, menuVX, menuVY,
          [&]{ PlaySound(clickSound); menuType = Type::MENU; inMenu = true; } },

        { resumeButton1, resumeButton2, 360.0f, 300.0f,
          [&]{ PlaySound(clickSound); menuType = Type::PLAYS; inMenu = false; } },
    };

    processButtons(buttons, mousePos);
}

void Menu::updateControls(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    DrawTextureFullscreen(controlMenu);
    inMenu = true;

    float backVY = BottomVY(backButton1);
    std::vector<Button> buttons = {
        { backButton1, backButton2, 10.0f, backVY,
          [&]{ PlaySound(clickSound); menuType = Type::MENU; } },
    };

    processButtons(buttons, mousePos);

    if (IsKeyPressed(KEY_ESCAPE))
        menuType = Type::MENU;
}

void Menu::updateDead(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    DrawTextureFullscreen(deadMenu);
    inMenu = true;

    if (!played)
    {
        SetSoundVolume(loseSound, 0.09f);
        PlaySound(loseSound);
        played = true;
    }

    float menuVX = RightVX(menuButton1);
    float menuVY = BottomVY(menuButton1);

    std::vector<Button> buttons = {
        { replayButton1, replayButton2, 360.0f, 300.0f,
          [&]{ played = false; PlaySound(clickSound); menuType = Type::PLAYS; } },

        { menuButton1, menuButton2, menuVX, menuVY,
          [&]{ played = false; PlaySound(clickSound); menuType = Type::MENU; inMenu = true; } },
    };

    processButtons(buttons, mousePos);
}

void Menu::updateWin(Type& menuType, Vector2 mousePos, bool& inMenu)
{
    DrawTextureFullscreen(winMenu);
    inMenu = true;

    if (!played)
    {
        SetSoundVolume(winSound, 0.10f);
        PlaySound(winSound);
        played = true;
    }

    float menuVX = RightVX(menuButton1);
    float menuVY = BottomVY(menuButton1);
    float exitVX = kVirtualScreenWidth - exitButton1.width * 2.0f - 5.0f;
    float exitVY = BottomVY(exitButton1);

    std::vector<Button> buttons = {
        { replayButton1, replayButton2, 360.0f, 300.0f,
          [&]{ played = false; PlaySound(clickSound); menuType = Type::PLAYS; } },

        { menuButton1, menuButton2, menuVX, menuVY,
          [&]{ played = false; PlaySound(clickSound); menuType = Type::MENU; inMenu = true; } },

        { exitButton1, exitButton2, exitVX, exitVY,
          [&]{ PlaySound(clickSound); CloseWindow(); } },
    };

    processButtons(buttons, mousePos);
}
