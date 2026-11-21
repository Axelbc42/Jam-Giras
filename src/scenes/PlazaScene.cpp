#include "PlazaScene.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

namespace
{
    const Color kTintNormal = { 200, 200, 200, 255 };
}

void PlazaScene::Init()
{
    AssetManager* assets = AssetManager::Instance();
    m_plaza = Create<Clickable>();
    m_item = Create<Draggable>();

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

    m_item->SetTexture(assets->GetTexture(Textures::ITEM_KEY));  
    m_item->SetRect(Rectangle{ 100.0f, 300.0f, 80.0f, 80.0f });
    m_item->SetOnDrop([this]() {
        const Rectangle& r = m_item->GetRect();
        const Vector2 centro = { r.x + r.width / 2.0f, r.y + r.height / 2.0f };

        if (CheckCollisionPointRec(centro, m_plaza->GetRect()))
        {
            m_message = "Has soltado el objeto en la plaza";
        }
        else
        {
            m_item->ReturnToStart();
            m_message = "Aqui no se puede soltar";
        }
        m_messageTimer = 2.0f;
        });

    m_message = "Haz clic en la plaza";
    m_messageTimer = 0.0f;
}

void PlazaScene::Update(float dt)
{
    if (m_messageTimer > 0.0f)
        m_messageTimer -= dt;

    Scene::Update(dt);   

    m_plaza->SetTint(m_plaza->IsHovered() ? WHITE : kTintNormal);
    if (!itemOcupado)
        m_plaza.Update(dt);
}

void PlazaScene::Draw()
{
    Scene::Draw();  

    const Rectangle& r = m_plaza->GetRect();
    DrawTextEx(m_font, "PLAZA", Vector2{ r.x, r.y - 50 }, 50.0f, 2.0f, RAYWHITE);

    DrawText(m_message.c_str(), 20, GameConstants::kScreenHeight - 50, 24, RAYWHITE);
    DrawText("F1: panel de depuracion", 20, 20, 20, LIGHTGRAY);
}

void PlazaScene::Release()
{
}

void PlazaScene::DrawDebugUI()
{
    ImGui::Separator();
    ImGui::Text("Plaza");

    Rectangle r = m_plaza->GetRect();
    ImGui::DragFloat("x", &r.x, 1.0f);
    ImGui::DragFloat("y", &r.y, 1.0f);
    ImGui::DragFloat("ancho", &r.width, 1.0f, 10.0f, 1280.0f);
    ImGui::DragFloat("alto", &r.height, 1.0f, 10.0f, 720.0f);
    m_plaza->SetRect(r);

    ImGui::Separator();
    ImGui::Text("Objeto arrastrable");
    ImGui::Text("Arrastrando: %s", m_item->IsDragging() ? "si" : "no");
    if (ImGui::Button("Devolver al inicio"))
        m_item->SetRect(Rectangle{ 100.0f, 300.0f, 80.0f, 80.0f });
}