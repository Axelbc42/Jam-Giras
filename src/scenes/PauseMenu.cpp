#include "PauseMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

void PauseMenu::Init()
{
    clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);

    _pauseBackground.SetActive(true);
    _pauseBackground.SetTexture(AssetManager::Instance()->GetTexture(Textures::BUTON1));
    _pauseBackground.SetRect(Rectangle{ (GameConstants::kScreenWidth / 2) - 180,
                                    (GameConstants::kScreenHeight / 2) - 320,
                                    360, 640 });
    _pauseBackground.SetTint(NORMAL_INK);


    _reaunarButton.SetActive(true);
    _reaunarButton.SetTexture(AssetManager::Instance()->GetTexture(Textures::BUTON1));
    _reaunarButton.SetRect(Rectangle{ (GameConstants::kScreenWidth / 2) - 150, (GameConstants::kScreenHeight / 2), 300, 100 });
    _reaunarButton.SetTint(NORMAL_INK);
    _reaunarButton.ResetCallbacks();
    _reaunarButton.AddCallback([this]() {
        AudioManager::Instance()->PlaySound(clickSound);
        SceneManager::Instance()->ChangeToLastScene();
        SceneManager::Instance()->ChangePauseMenu();
        });

    _salirButton.SetActive(true);
    _salirButton.SetTexture(AssetManager::Instance()->GetTexture(Textures::BUTON1));
    _salirButton.SetRect(Rectangle{ (GameConstants::kScreenWidth / 2) - 150, (GameConstants::kScreenHeight / 2) + 150, 300, 100 });
    _salirButton.SetTint(NORMAL_INK);
    _salirButton.ResetCallbacks();
    _salirButton.AddCallback([this]() {
        TraceLog(LOG_INFO, "Aqui tendrias q salir");
        });
}

void PauseMenu::Update(float dt)
{
    _reaunarButton.Update(dt);
    _salirButton.Update(dt);
}

void PauseMenu::Draw()
{
    DrawRectangle(0, 0, GameConstants::kScreenWidth, GameConstants::kScreenWidth, Color { 0, 0, 0, 100 });

    _pauseBackground.Draw();
    _reaunarButton.Draw();
    _salirButton.Draw();
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