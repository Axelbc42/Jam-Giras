#include "PauseMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

void PauseMenu::Init()
{
    _clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
    _font = AssetManager::Instance()->GetFont(Fonts::MAIN);

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
        AudioManager::Instance()->PlaySound(_clickSound);
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

    // Ventana de ImGui encima del panel
    ImGui::SetNextWindowPos(ImVec2(GameConstants::kScreenWidth / 2 - 150, GameConstants::kScreenHeight / 2 - 250));
    ImGui::SetNextWindowSize(ImVec2(360, 640));
    ImGui::Begin("##pause_options", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));         // barra
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.55f, 0.55f, 0.55f, 1.0f));      // bolita
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 20.0f);                              // grosor de la bolita
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 10));                     // altura de la barra

    float music = AudioManager::Instance()->GetMusicVolume() * 100.0f;
    ImGui::Text("Musica");
    ImGui::SetNextItemWidth(285);
    if (ImGui::SliderFloat("##Musica", &music, 0.0f, 100.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        AudioManager::Instance()->SetMusicVolumeAM(music / 100.0f);

    float sound = AudioManager::Instance()->GetSFXVolume() * 100.0f;
    ImGui::Text("Sonido");
    ImGui::SetNextItemWidth(285);
    if (ImGui::SliderFloat("##Sonido", &sound, 0.0f, 100.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        AudioManager::Instance()->SetSFXVolumeAM(sound / 100.0f);

    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(5);

    ImGui::End();
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