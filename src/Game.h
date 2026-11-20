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

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

private:
    Game() = default;
    ~Game();

    bool initWindow();
    void update(float dt);
    void draw();
    void drawDebugUI();

    static Game* s_instance;

    bool m_windowReady = false;

    // --- Depuracion ---
    bool m_debug = true;         // F1 muestra/oculta el panel de ImGui
    bool m_showImGuiDemo = false;

    // --- Prueba temporal: un hotspot clicable ---
    Rectangle m_hotspot{ 0, 0, 0, 0 };
    bool m_hotspotHover = false;
    std::string m_message;
    float m_messageTimer = 0.0f;
};