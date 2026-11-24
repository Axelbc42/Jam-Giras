#pragma once
#include "Scene.h"
#include "raylib.h"
#include <functional>

using BtnCallback = std::function<void()>;

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
    struct Btn {
        std::string text;
        Rectangle rect;
        Texture2D bg;            // puede o no tener fondo
        BtnCallback onClick;
        bool hover = false;
        Color textC = BLACK;
    };

    void createBtn(const std::string& text, Rectangle rect,
        std::function<void()> onClick, Texture2D bg = {}, Color textC=BLACK);

    std::vector<Btn> _btns;
private:
    Texture2D _background{};
    Rectangle _backgroundHS{};
    Sound clickSound{};
    Font m_font{};
};

