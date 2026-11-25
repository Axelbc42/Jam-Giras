#pragma once
#include "Scene.h"
#include "raylib.h"
class CallejonNorte : public Scene
{
public:
    CallejonNorte(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

private:

};
