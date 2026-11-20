#include "Game.h"

#include "imgui.h"
#include "rlImGui.h"
#include "core/AssetManager.h"
#include "core/AudioManager.h"

namespace
{
    const int kScreenWidth = 1280;
    const int kScreenHeight = 720;
    const char* kTitle = "Jam-Giras";
}

Game* Game::s_instance = nullptr;

// ---------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------
bool Game::Init()
{
    if (s_instance != nullptr)
        return true;

    s_instance = new Game();
    if (!s_instance->initWindow())
    {
        Release();
        return false;
    }
    return true;
}

Game* Game::Instance()
{
    return s_instance;
}

void Game::Release()
{
    delete s_instance;
    s_instance = nullptr;
}

Game::~Game()
{
    if (m_windowReady)
    {
        AssetManager::Release();   // 1) recursos (necesitan la ventana y el audio abiertos)
        AudioManager::Release();
        CloseAudioDevice();        // 2) audio
        rlImGuiShutdown();         // 3) ImGui
        CloseWindow();             // 4) ventana
    }
}

// ---------------------------------------------------------------
// Inicializacion
// ---------------------------------------------------------------
bool Game::initWindow()
{
    InitWindow(kScreenWidth, kScreenHeight, kTitle);
    if (!IsWindowReady())
        return false;

    AudioManager::Instance()->Init();
    SetTargetFPS(60);
    rlImGuiSetup(true);   
    m_windowReady = true;
    return true;
}

void Game::initGame()
{
    clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
    TraceLog(LOG_INFO, "Sound loaded: %d", clickSound.frameCount);

    m_hotspotTexture = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_PLAZA);
    m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);

    m_hotspot = Rectangle{ 400.0f, 200.0f, 500.0f, 350.0f };
    m_message = "Haz clic en la plaza";
}
// ---------------------------------------------------------------
// Bucle principal
// ---------------------------------------------------------------
void Game::start()
{
    while (!WindowShouldClose())
    {
        update(GetFrameTime());
        draw();
    }
}

void Game::update(float dt)
{
    if (IsKeyPressed(KEY_F1))
        m_debug = !m_debug;

    if (m_messageTimer > 0.0f)
        m_messageTimer -= dt;

    // Si el raton esta sobre una ventana de ImGui, el juego no recibe el clic.
    const bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;

    const Vector2 mouse = GetMousePosition();
    m_hotspotHover = !imguiCapturaRaton && CheckCollisionPointRec(mouse, m_hotspot);

    if (m_hotspotHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        AudioManager::Instance()->PlaySound(clickSound);
        m_message = "Has hecho clic en la plaza";
        m_messageTimer = 2.0f;
    }
}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(Color{ 40, 44, 52, 255 });

    // --- Mundo del juego ---
    //DrawRectangleRec(m_hotspot, m_hotspotHover ? Color{ 200, 160, 60, 255 } : Color{ 120, 90, 40, 255 });

    Rectangle origen = { 0.0f, 0.0f, (float)m_hotspotTexture.width, (float)m_hotspotTexture.height };
    Color tinte = m_hotspotHover ? WHITE : Color{ 200, 200, 200, 255 };
    DrawTexturePro(m_hotspotTexture, origen, m_hotspot, Vector2{ 0, 0 }, 0.0f, tinte);

    DrawTextEx(m_font, "PLAZA", Vector2{ m_hotspot.x, m_hotspot.y - 50 }, 50.0f, 2.0f, RAYWHITE);

    // --- Interfaz del juego ---
    DrawText(m_message.c_str(), 20, kScreenHeight - 50, 24, RAYWHITE);
    DrawText("F1: panel de depuracion", 20, 20, 20, LIGHTGRAY);

    // --- Depuracion con ImGui ---
    if (m_debug)
    {
        rlImGuiBegin();
        drawDebugUI();
        rlImGuiEnd();
    }
    EndDrawing();
}

void Game::drawDebugUI()
{
    ImGui::Begin("Depuracion");
    ImGui::Text("FPS: %d", GetFPS());

    const Vector2 mouse = GetMousePosition();
    ImGui::Text("Raton: %.0f, %.0f", mouse.x, mouse.y);

    ImGui::Separator();
    ImGui::Text("Hotspot de prueba");
    ImGui::DragFloat("x", &m_hotspot.x, 1.0f);
    ImGui::DragFloat("y", &m_hotspot.y, 1.0f);
    ImGui::DragFloat("ancho", &m_hotspot.width, 1.0f, 10.0f, 1280.0f);
    ImGui::DragFloat("alto", &m_hotspot.height, 1.0f, 10.0f, 720.0f);

    ImGui::Separator();
    ImGui::Checkbox("Demo de ImGui", &m_showImGuiDemo);
    ImGui::End();

    if (m_showImGuiDemo)
        ImGui::ShowDemoWindow(&m_showImGuiDemo);
}