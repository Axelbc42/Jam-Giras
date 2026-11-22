#pragma once
#include "Scene.h"
#include "raylib.h"
#include "../entities/Clickable.h"

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
    const Color NORMAL_INK = { 200, 200, 200, 255 };
    GameObject _pauseBackground{};

    Clickable _reaunarButton{};
    Clickable _salirButton{};

    Sound _clickSound{};
    Font _font{};
};
