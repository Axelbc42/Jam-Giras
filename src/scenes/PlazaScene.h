#pragma once

#include "Scene.h"
#include "raylib.h"

class PlazaScene : public Scene
{
public:
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void Release() override;

private:
    Texture2D m_background{};
    Rectangle m_hotspot{};
    bool m_hotspotHover = false;
};