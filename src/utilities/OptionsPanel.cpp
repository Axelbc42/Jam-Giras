#include "OptionsPanel.h"
#include "imgui.h"
#include "../core/AudioManager.h"
#include "../Game.h"

void OptionsPanel::Draw(float x, float y, float w, float h)
{
    auto* g = Game::Instance();
    const float s = g->GameScale();
    const Vector2 p = g->GameToWindow(x, y);

    ImGui::SetNextWindowPos(ImVec2(p.x, p.y));
    ImGui::SetNextWindowSize(ImVec2(w * s, h * s));
    ImGui::Begin("##options_panel", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowFontScale(s);

    // Estilo papel pareido al menu
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.16f, 0.12f, 0.24f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.16f, 0.12f, 0.24f, 0.12f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.16f, 0.12f, 0.24f, 0.20f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.16f, 0.12f, 0.24f, 0.28f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.16f, 0.12f, 0.24f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.66f, 0.16f, 0.16f, 1.f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f * s);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 2.f * s);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 18.f * s);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4 * s, 8 * s));

    float music = AudioManager::Instance()->GetMusicVolume() * 100.f;
    ImGui::Text("Música");
    ImGui::SetNextItemWidth((w - 20.f) * s);
    if (ImGui::SliderFloat("##Música", &music, 0.f, 100.f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        AudioManager::Instance()->SetMusicVolumeAM(music / 100.f);

    float sound = AudioManager::Instance()->GetSFXVolume() * 100.f;
    ImGui::Text("Sonido");
    ImGui::SetNextItemWidth((w - 20.f) * s);
    if (ImGui::SliderFloat("##Sonido", &sound, 0.f, 100.f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        AudioManager::Instance()->SetSFXVolumeAM(sound / 100.f);

    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(6);
    ImGui::End();
}