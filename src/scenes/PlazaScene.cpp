#include "PlazaScene.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

void PlazaScene::Init()
{
    clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
    TraceLog(LOG_INFO, "Sound loaded: %d", clickSound.frameCount);

    m_hotspotTexture = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_PLAZA);
    m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);

    m_hotspot = Rectangle{ 400.0f, 200.0f, 500.0f, 350.0f };
    m_message = "Haz clic en la plaza";
}

void PlazaScene::Update(float dt)
{
    if (m_messageTimer > 0.0f)
        m_messageTimer -= dt;

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

void PlazaScene::Draw()
{
    Rectangle origen = { 0.0f, 0.0f, (float)m_hotspotTexture.width, (float)m_hotspotTexture.height };
    Color tinte = m_hotspotHover ? WHITE : Color{ 200, 200, 200, 255 };
    DrawTexturePro(m_hotspotTexture, origen, m_hotspot, Vector2{ 0, 0 }, 0.0f, tinte);

    DrawTextEx(m_font, "PLAZA", Vector2{ m_hotspot.x, m_hotspot.y - 50 }, 50.0f, 2.0f, RAYWHITE);

    // --- Interfaz del juego ---
    DrawText(m_message.c_str(), 20, GameConstants::kScreenHeight - 50, 24, RAYWHITE);
    DrawText("F1: panel de depuracion", 20, 20, 20, LIGHTGRAY);
}

void PlazaScene::Release()
{
    // Liberar recursos propios de esta escena
}

void PlazaScene::DrawDebugUI()
{
    ImGui::Separator();
    ImGui::Text("Hotspot de prueba");
    ImGui::DragFloat("x", &m_hotspot.x, 1.0f);
    ImGui::DragFloat("y", &m_hotspot.y, 1.0f);
    ImGui::DragFloat("ancho", &m_hotspot.width, 1.0f, 10.0f, 1280.0f);
    ImGui::DragFloat("alto", &m_hotspot.height, 1.0f, 10.0f, 720.0f);
}