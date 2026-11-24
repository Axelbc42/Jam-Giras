#pragma once

#include <string>
#include "raylib.h"

class Game
{
public:
    static bool Init();          // Crea la instancia y la ventana. false si falla
    static Game* Instance();     // Devuelve la instancia (nullptr si no hay)
    static void Release();       // Destruye la instancia y cierra la ventana

    void initGame();             // Carga/prepara el contenido del juego
    void start();                // Bucle principal

    void RequestQuit() { m_quit = true; } //Para que elmenu pueda salir del juego

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

private:
    Game() = default;
    ~Game();

    bool initWindow();
    void update(float dt);
    void draw();
    void drawDebugUI();
    void updateViewport();

    static Game* s_instance;

    bool m_windowReady = false;
    bool m_quit = false;
    RenderTexture2D m_target{};
    float m_scale = 1.f, m_offX = 0.f, m_offY = 0.f;


    // --- Depuracion ---
    bool m_debug = true;         // F1 muestra/oculta el panel de ImGui
    bool m_showImGuiDemo = false;
};