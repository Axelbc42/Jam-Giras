#pragma once

#include <string>
#include "raylib.h"
#include "Scene.h"

class PlazaScene : public Scene
{
public:
    PlazaScene(const std::string& name) : Scene(name) {}

    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void Release() override;
    void DrawDebugUI() override;

private:
    Clickable* m_plaza = nullptr;
    Draggable* m_item = nullptr;

    Sound clickSound{};
    Font m_font{};

    std::string m_message;
    float m_messageTimer = 0.0f;
};