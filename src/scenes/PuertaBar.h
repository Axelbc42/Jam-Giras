#pragma once
#include "Scene.h"
#include "raylib.h"

class PuertaBar : public Scene
{
public:
    PuertaBar(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

private:
    Clickable _puertaBar;
};
