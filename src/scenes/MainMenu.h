#pragma once
#include "Scene.h"
#include "raylib.h"
#include <functional>
#include "../entities/Button.h"

class MainMenu : public Scene
{
public:
    MainMenu(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

private:
    Texture2D _background{};
    Rectangle _backgroundHS{};
    Sound clickSound{};
    Font m_font{};
};

