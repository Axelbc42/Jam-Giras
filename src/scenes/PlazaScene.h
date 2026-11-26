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
    void OnEnter() override;
    void Update(float dt) override;
    void OnExit() override; //Provisional de momento para cerrar los textos

private:
    GameObject* m_plaza = nullptr;
    Clickable* _diary = nullptr;
    Font m_font{};

    bool tutorialShown = false;
    bool pendingTutorial = false;
};