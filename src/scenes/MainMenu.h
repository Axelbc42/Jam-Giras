#pragma once
#include "Scene.h"
#include "raylib.h"
#include <functional>
#include "../entities/Button.h"

class MainMenu : public Scene
{
public:
    MainMenu(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

    void SetBotones();
private:
    Texture2D _background{};
    Rectangle _backgroundHS{};
    Sound _clickSound{};
    Font m_font{};
    Color _inkP{};

    bool _enOpciones = false;
    Button* _bJugar = nullptr;
    Button* _bOpciones = nullptr;
    Button* _bCreditos = nullptr;
    Button* _bSalir = nullptr;
    Button* _bVolver = nullptr;
};

