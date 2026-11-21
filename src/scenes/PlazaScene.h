#pragma once

#include <string>
#include "raylib.h"
#include "Scene.h"
#include "../entities/Clickable.h"
#include "../entities/Draggable.h"

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
    Clickable m_plaza;    
    Draggable m_item;     

    Sound clickSound{};
    Font m_font{};

    std::string m_message;
    float m_messageTimer = 0.0f;
};