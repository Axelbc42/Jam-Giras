#pragma once
#include "Scene.h"
#include "raylib.h"

class PauseMenu : public Scene
{
public:
    PauseMenu(std::string name) : Scene(name) {}
    void Init() override;
    void Draw() override;

private:
    Rectangle _panel{};
    Font _font{};
    Color _inkP{};
};
