#include "CallejonOeste.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/DialogueManager.h"
#include "../utilities/GameConstants.h"
#include "../entities/items/dialogueInteractuable.h"
#include "imgui.h"

namespace
{
    const Color kTintNormal = { 200, 200, 200, 255 };
}

void CallejonOeste::Init()
{
    AssetManager* assets = AssetManager::Instance();

    if (m_plaza == nullptr)
        m_plaza = new DialogueInteractuable(FileName::NPC_1, true, true);

    clickSound = assets->GetSound(Sounds::CLICK);
    m_font = assets->GetFont(Fonts::MAIN);

    m_plaza->SetTexture(assets->GetTexture(Textures::BACKGROUND_PLAZA));
    m_plaza->SetRect(Rectangle{ 400.0f, 200.0f, 500.0f, 350.0f });
    m_plaza->SetTint(kTintNormal);

    m_plaza->AddCallback([this]() {
        AudioManager::Instance()->PlaySound(clickSound);
        });

    Adopt(m_plaza);
}

void CallejonOeste::Update(float dt)
{
    Scene::Update(dt);

    m_plaza->SetTint(m_plaza->IsHovered() ? WHITE : kTintNormal);
}

void CallejonOeste::Draw()
{
    Scene::Draw();

    const Rectangle& r = m_plaza->GetRect();
    DrawTextEx(m_font, "PLAZA", Vector2{ r.x, r.y - 50 }, 50.0f, 2.0f, RAYWHITE);

    DrawText("F1: panel de depuracion", 20, 20, 20, LIGHTGRAY);
}

void CallejonOeste::Release()
{
    delete m_plaza;
}

void CallejonOeste::DrawDebugUI()
{
    ImGui::Separator();
    ImGui::Text("Plaza (clicable)");

    Rectangle r = m_plaza->GetRect();
    ImGui::DragFloat("x", &r.x, 1.0f);
    ImGui::DragFloat("y", &r.y, 1.0f);
    ImGui::DragFloat("ancho", &r.width, 1.0f, 10.0f, 1280.0f);
    ImGui::DragFloat("alto", &r.height, 1.0f, 10.0f, 720.0f);
    m_plaza->SetRect(r);
}