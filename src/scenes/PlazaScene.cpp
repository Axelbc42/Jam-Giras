#include "PlazaScene.h"
#include "../core/AssetManager.h"

void PlazaScene::Init()
{
    m_background = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_PLAZA);

    m_hotspot = { 400.0f, 200.0f, 500.0f, 350.0f };
}

void PlazaScene::Update(float dt)
{
    Vector2 mouse = GetMousePosition();

    m_hotspotHover = CheckCollisionPointRec(mouse, m_hotspot);

    if (m_hotspotHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        // Interaccion 
    }
}

void PlazaScene::Draw()
{
    Rectangle origen = {
        0.0f, 0.0f,
        (float)m_background.width,
        (float)m_background.height
    };

    Color tinte = m_hotspotHover ? WHITE : Color{ 200, 200, 200, 255 };

    DrawTexturePro(m_background, origen, m_hotspot, Vector2{ 0, 0 }, 0.0f, tinte);

    DrawText("PLAZA", (int)m_hotspot.x, (int)m_hotspot.y - 50, 50, RAYWHITE);
}

void PlazaScene::Release()
{
    // Liberar recursos propios de esta escena
}