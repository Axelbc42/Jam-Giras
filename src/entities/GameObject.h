#pragma once

#include "raylib.h"

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void Update(float dt) {}
    virtual void DrawDebugUI() {}

    virtual void Draw()
    {
        if (!m_active || m_texture.id == 0)
            return;

        const Rectangle origen = { 0.0f, 0.0f, (float)m_texture.width, (float)m_texture.height };
        DrawTexturePro(m_texture, origen, m_rect, Vector2{ 0, 0 }, 0.0f, m_tint);
    }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    const Rectangle& GetRect() const { return m_rect; }
    void SetRect(const Rectangle& rect) { m_rect = rect; }
    void SetPosition(float x, float y) { m_rect.x = x; m_rect.y = y; }

    void SetTexture(const Texture& texture) { m_texture = texture; }
    void SetTint(const Color& tint) { m_tint = tint; }

protected:   
    Rectangle m_rect{};
    Texture m_texture{};
    Color m_tint = WHITE;
    bool m_active = true;
};