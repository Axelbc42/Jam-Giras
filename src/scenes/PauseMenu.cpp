#include "PauseMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

void PauseMenu::Init()
{
    clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
    _pauseBackground = AssetManager::Instance()->GetTexture(Textures::BUTON1);
    _pauseBackgroundHS = Rectangle{ (GameConstants::kScreenWidth / 2) - 180,
                                    (GameConstants::kScreenHeight / 2) - 320,
                                    360, 640 };

    _reaunarButton = AssetManager::Instance()->GetTexture(Textures::BUTON1);
    _reaunarButtonHS = Rectangle{ (GameConstants::kScreenWidth / 2) - 150,
                                  (GameConstants::kScreenHeight / 2), 
                                  300, 100 };

    _salirButton = AssetManager::Instance()->GetTexture(Textures::BUTON1);
    _salirButtonHS = Rectangle{ (GameConstants::kScreenWidth / 2) - 150,
                                (GameConstants::kScreenHeight / 2) + 150, 
                                300, 100 };
}

void PauseMenu::Update(float dt)
{
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, _reaunarButtonHS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        AudioManager::Instance()->PlaySound(clickSound);
        SceneManager::Instance()->ChangeToLastScene();
        SceneManager::Instance()->ChangePauseMenu();
    }

    if (CheckCollisionPointRec(mouse, _salirButtonHS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

    }
}

void PauseMenu::Draw()
{
    DrawRectangle(0, 0, GameConstants::kScreenWidth, GameConstants::kScreenWidth, Color { 0, 0, 0, 100 });

    Rectangle pauseBackground = { 0.0f, 0.0f, (float)_pauseBackground.width, (float)_pauseBackground.height };
    DrawTexturePro(_pauseBackground, pauseBackground, _pauseBackgroundHS, Vector2{ 0, 0 }, 0.0f, WHITE);

    Rectangle pauseButonRec = { 0.0f, 0.0f, (float)_reaunarButton.width, (float)_reaunarButton.height };
    DrawTexturePro(_reaunarButton, pauseButonRec, _reaunarButtonHS, Vector2{ 0, 0 }, 0.0f, WHITE);

    Rectangle exitButonRec = { 0.0f, 0.0f, (float)_salirButton.width, (float)_salirButton.height };
    DrawTexturePro(_salirButton, exitButonRec, _salirButtonHS, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void PauseMenu::Release()
{
    // Liberar recursos propios de esta escena
}

void PauseMenu::DrawDebugUI()
{
    //ImGui::Separator();
    //ImGui::Text("Hotspot de prueba");
    //ImGui::DragFloat("x", &_backgroundHS.x, 1.0f);
    //ImGui::DragFloat("y", &_backgroundHS.y, 1.0f);
    //ImGui::DragFloat("ancho", &_backgroundHS.width, 1.0f, 10.0f, 1280.0f);
    //ImGui::DragFloat("alto", &_backgroundHS.height, 1.0f, 10.0f, 720.0f);
}