#pragma once
#include "Scene.h"
#include "raylib.h"

class MainMenu : public Scene
{
public:
    MainMenu(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;
private:
    Texture2D _background{};
    Rectangle _backgroundHS{};
    Texture2D _button{};
    Rectangle _buttonHS{};
    bool _buttonHSHover = false;

    std::string nextScene = "Plaza";
    Sound clickSound;
    Font m_font{};
    std::string m_message;
};

