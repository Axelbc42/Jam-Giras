#pragma once

#include "Scene.h"
#include "raylib.h"

class PlazaScene : public Scene
{
public:
    PlazaScene(std::string name) : Scene(name){}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

private:
    Rectangle m_hotspot{};
    Texture m_hotspotTexture{};
    Font m_font{};
    Sound clickSound;
    bool m_hotspotHover = false;
    std::string m_message;
    float m_messageTimer = 0.0f;
};