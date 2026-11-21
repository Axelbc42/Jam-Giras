#pragma once
#include "Scene.h"
#include "raylib.h"

class PauseMenu : public Scene
{
public:
    PauseMenu(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

private:
    Texture2D _pauseBackground{};
    Rectangle _pauseBackgroundHS{};

    Texture2D _reaunarButton{};
    Rectangle _reaunarButtonHS{};
    Texture2D _salirButton{};
    Rectangle _salirButtonHS{};

    Sound clickSound;
    Font m_font{};
};
