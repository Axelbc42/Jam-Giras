#pragma once

#include "raylib.h"
#include "Scene.h"

class PlazaScene : public Scene
{
public:
    PlazaScene(const std::string& name) : Scene(name) {}

    void Init() override;
    void Draw() override;
    void Release() override;
    void DrawDebugUI() override;

private:
    GameObject* m_plaza = nullptr;
    Font m_font{};
};