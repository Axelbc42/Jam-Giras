#pragma once
#include <string>
#include "raylib.h"
#include "Scene.h"

class CallejonOeste
    : public Scene
{
public:
    CallejonOeste(const std::string& name) : Scene(name) {}

    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void Release() override;
    void DrawDebugUI() override;

private:
    Clickable* m_plaza = nullptr;

    Sound clickSound{};
    Font m_font{};

};