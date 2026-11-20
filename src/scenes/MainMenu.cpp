#include "MainMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

void MainMenu::Init()
{
    clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
    TraceLog(LOG_INFO, "Sound loaded: %d", clickSound.frameCount);

    _background = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_MENU);
    _button = AssetManager::Instance()->GetTexture(Textures::BUTON1);
    m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);

    _backgroundHS = Rectangle{ 0.0f, 0, 1280.0f, 720.0f };
    _buttonHS = Rectangle{ 640.0f, 360, 360, 160.0f };
    m_message = "Haz clic en el boton \n67 skibidy sigma \nperdon";
}

void MainMenu::Update(float dt)
{
    bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;
    Vector2 mouse = GetMousePosition();

    _buttonHSHover = !imguiCapturaRaton && CheckCollisionPointRec(mouse, _buttonHS);

    if (_buttonHSHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        AudioManager::Instance()->PlaySound(clickSound);
        SceneManager::Instance()->ChangeScene(nextScene);
    }
}

void MainMenu::Draw()
{
    Rectangle origenFondo = { 0.0f, 0.0f, (float)_background.width, (float)_background.height };
    Rectangle origenBoton = { 0.0f, 0.0f, (float)_button.width,     (float)_button.height };
    Color tinte = _buttonHSHover ? WHITE : Color{ 200, 200, 200, 255 };

    DrawTexturePro(_background, origenFondo, _backgroundHS, Vector2{ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(_button, origenBoton, _buttonHS, Vector2{ 0, 0 }, 0.0f, tinte);

    // --- Interfaz del juego ---
    DrawText(m_message.c_str(), _buttonHS.x + 60, _buttonHS.y + 30, 24, BLACK);
    DrawText("F1: panel de depuracion", 20, GameConstants::kScreenHeight - 50, 24, BLACK);
}

void MainMenu::Release()
{
    // Liberar recursos propios de esta escena
}

void MainMenu::DrawDebugUI()
{
    ImGui::Separator();
    ImGui::Text("Hotspot de prueba");
    ImGui::DragFloat("x", &_backgroundHS.x, 1.0f);
    ImGui::DragFloat("y", &_backgroundHS.y, 1.0f);
    ImGui::DragFloat("ancho", &_backgroundHS.width, 1.0f, 10.0f, 1280.0f);
    ImGui::DragFloat("alto", &_backgroundHS.height, 1.0f, 10.0f, 720.0f);
}