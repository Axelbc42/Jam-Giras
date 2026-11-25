#include "CallejonOeste.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../utilities/GameConstants.h"
#include "imgui.h"

namespace
{
    const Color kTintNormal = { 200, 200, 200, 255 };
}

void CallejonOeste::Init()
{
    AssetManager* assets = AssetManager::Instance();

    if (m_plaza == nullptr)
        m_plaza = Create<Clickable>();

    clickSound = assets->GetSound(Sounds::CLICK);
    m_font = assets->GetFont(Fonts::MAIN);

    m_plaza->SetTexture(assets->GetTexture(Textures::BACKGROUND_PLAZA));
    m_plaza->SetRect(Rectangle{ 400.0f, 200.0f, 500.0f, 350.0f });
    m_plaza->SetTint(kTintNormal);

    m_plaza->ResetCallbacks();
    m_plaza->AddCallback([this]() {
        AudioManager::Instance()->PlaySound(clickSound);
        m_message = "Has hecho clic en la plaza";
        m_messageTimer = 2.0f;
        });

    m_message = "Haz clic en la plaza";
    m_messageTimer = 0.0f;
}

void CallejonOeste::Update(float dt)
{
    if (m_messageTimer > 0.0f)
        m_messageTimer -= dt;

    Scene::Update(dt);

    m_plaza->SetTint(m_plaza->IsHovered() ? WHITE : kTintNormal);
}

void CallejonOeste::Draw()
{
    Scene::Draw();

    const Rectangle& r = m_plaza->GetRect();
    DrawTextEx(m_font, "PLAZA", Vector2{ r.x, r.y - 50 }, 50.0f, 2.0f, RAYWHITE);

    DrawText(m_message.c_str(), 20, GameConstants::kScreenHeight - 50, 24, RAYWHITE);
    DrawText("F1: panel de depuracion", 20, 20, 20, LIGHTGRAY);
}

void CallejonOeste::Release()
{
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