#include "PauseMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"
#include "../Game.h"

#include "imgui.h"

void PauseMenu::Init()
{
    _clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
    _font = AssetManager::Instance()->GetFont(Fonts::MAIN);

    _pauseBackground.SetActive(true);
    _pauseBackground.SetTexture(AssetManager::Instance()->GetTexture(Textures::BUTON1));
    _pauseBackground.SetRect(Rectangle{ (GameConstants::kScreenWidth / 2) - 180, (GameConstants::kScreenHeight / 2) - 320, 360, 640 });
    _pauseBackground.SetTint(NORMAL_INK);

    _reaunarButton.SetActive(true);
    _reaunarButton.SetTexture(AssetManager::Instance()->GetTexture(Textures::BUTON1));
    _reaunarButton.SetRect(Rectangle{ (GameConstants::kScreenWidth / 2) - 150, (GameConstants::kScreenHeight / 2), 300, 100 });
    //_reaunarButton.SetTint(NORMAL_INK);
    _reaunarButton.ResetCallbacks();
    _reaunarButton.AddCallback([this]() {
        AudioManager::Instance()->PlaySound(_clickSound);
        SceneManager::Instance()->ChangeToLastScene();
        SceneManager::Instance()->ChangePauseMenu();
        });

    _salirButton.SetActive(true);
    _salirButton.SetTexture(AssetManager::Instance()->GetTexture(Textures::BUTON1));
    _salirButton.SetRect(Rectangle{ (GameConstants::kScreenWidth / 2) - 150, (GameConstants::kScreenHeight / 2) + 150, 300, 100 });
    //_salirButton.SetTint(NORMAL_INK);
    _salirButton.ResetCallbacks();
    _salirButton.AddCallback([this]() {
        Game::Instance()->RequestQuit();
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

    // texto centrado en el rect
    const float size = 24;
    const float spacing = 2;
    Vector2 ts1 = MeasureTextEx(_font, "Reanudar", size, spacing);
    Vector2 pos1 = { _reaunarButton.GetRect().x + (_reaunarButton.GetRect().width - ts1.x) / 2,
                    _reaunarButton.GetRect().y + (_reaunarButton.GetRect().height - ts1.y) / 2 };
    DrawTextEx(_font, "Reanudar", pos1, size, spacing, WHITE);

    Vector2 ts2 = MeasureTextEx(_font, "Salir", size, spacing);
    Vector2 pos2 = { _salirButton.GetRect().x + (_salirButton.GetRect().width - ts2.x) / 2,
                    _salirButton.GetRect().y + (_salirButton.GetRect().height - ts2.y) / 2 };
    DrawTextEx(_font, "Salir", pos2, size, spacing, WHITE);

    auto* g = Game::Instance();
    const float s = g->GameScale();
    // Ventana de ImGui encima del panel
    const Vector2 p = g->GameToWindow(GameConstants::kScreenWidth / 2.f - 150.f, GameConstants::kScreenHeight / 2.f - 250.f);
    ImGui::SetNextWindowPos(ImVec2(p.x,p.y));
    ImGui::SetNextWindowSize(ImVec2(360.f * s, 180.f * s));
    ImGui::Begin("##pause_options", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowFontScale(s);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));         // barra
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.55f, 0.55f, 0.55f, 1.0f));      // bolita
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f*s);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 8.0f * s);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 20.0f * s);                              // grosor de la bolita
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4 * s, 10 * s));                     // altura de la barra

    float music = AudioManager::Instance()->GetMusicVolume() * 100.0f;
    ImGui::Text("Musica");
    ImGui::SetNextItemWidth(285 * s);
    if (ImGui::SliderFloat("##Musica", &music, 0.0f, 100.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        AudioManager::Instance()->SetMusicVolumeAM(music / 100.0f);

    float sound = AudioManager::Instance()->GetSFXVolume() * 100.0f;
    ImGui::Text("Sonido");
    ImGui::SetNextItemWidth(285 * s);
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