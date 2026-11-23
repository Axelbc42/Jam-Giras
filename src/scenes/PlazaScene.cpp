#include "PlazaScene.h"
#include "../core/AssetManager.h"
#include "../utilities/GameConstants.h"
#include "../entities/items/door.h"
#include "../entities/items/key.h"
#include "../core/DoorManager.h"
#include "imgui.h"

void PlazaScene::Init()
{
    AssetManager* assets = AssetManager::Instance();
    DoorManager* dm = DoorManager::Instance();

    if (m_plaza == nullptr)
    {
        m_plaza = Create<GameObject>();
        m_plaza->SetTexture(assets->GetTexture(Textures::BACKGROUND_PLAZA));
        m_plaza->SetRect(Rectangle{ 400.0f, 200.0f, 500.0f, 350.0f });

        Door* d = dm->GetDoor(1);
        d->SetTexture(assets->GetTexture(Textures::CLOSED_DOOR));
        d->SetOpenTex(assets->GetTexture(Textures::OPENED_DOOR));
        d->SetRect(Rectangle{ 590.0f, 270.0f, 120.0f, 180.0f });
        Adopt(d);

        Key* k = dm->GetKey(0);
        k->SetTexture(assets->GetTexture(Textures::ITEM_KEY_1));
        k->SetRect(Rectangle{ 100.0f, 300.0f, 80.0f, 80.0f });
        Adopt(k);

        Key* k2 = dm->GetKey(1);
        k2->SetTexture(assets->GetTexture(Textures::ITEM_KEY_2));
        k2->SetRect(Rectangle{ 100.0f, 400.0f, 80.0f, 80.0f });
        Adopt(k2);
    }

    m_font = assets->GetFont(Fonts::MAIN);
}

void PlazaScene::Draw()
{
    Scene::Draw();

    const Rectangle& r = m_plaza->GetRect();
    DrawTextEx(m_font, "PLAZA", Vector2{ r.x, r.y - 50 }, 50.0f, 2.0f, RAYWHITE);

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
}