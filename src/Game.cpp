#include "Game.h"

#include "imgui.h"
#include "rlImGui.h"

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
        rlImGuiShutdown();
        CloseWindow();
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

    SetTargetFPS(60);
    rlImGuiSetup(true);   // true = tema oscuro
    m_windowReady = true;
    return true;
}

void Game::initGame()
{
    // Aqui iran la carga de recursos y la primera escena.
    m_hotspot = Rectangle{ 800.0f, 200.0f, 200.0f, 350.0f };
    m_message = "Haz clic en la puerta";
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
        m_message = "Has hecho clic en la puerta";
        m_messageTimer = 2.0f;
    }
}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(Color{ 40, 44, 52, 255 });

    // --- Mundo del juego ---
    DrawRectangleRec(m_hotspot, m_hotspotHover ? Color{ 200, 160, 60, 255 }
    : Color{ 120, 90, 40, 255 });
    DrawText("PUERTA", (int)m_hotspot.x + 50, (int)m_hotspot.y + 160, 24, RAYWHITE);

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