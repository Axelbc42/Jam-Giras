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
        const Rectangle destino = { m_rect.x + m_rect.width / 2.0f, m_rect.y + m_rect.height / 2.0f,
                                    m_rect.width, m_rect.height };
        const Vector2 pivote = { m_rect.width / 2.0f, m_rect.height / 2.0f };

        DrawTexturePro(m_texture, origen, destino, pivote, m_rotation, m_tint);
    }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    const Rectangle& GetRect() const { return m_rect; }
    void SetRect(const Rectangle& rect) { m_rect = rect; }
    void SetPosition(float x, float y) { m_rect.x = x; m_rect.y = y; }
    void SetSize(float w, float h) { m_rect.width = w; m_rect.height = h; }
    void SetRotation(float degrees) { m_rotation = degrees; } 

    void SetTexture(const Texture& texture) { m_texture = texture; }
    void SetTint(const Color& tint) { m_tint = tint; }

protected:   
    Rectangle m_rect{};
    float m_rotation = 0.0f;
    Texture m_texture{};
    Color m_tint = WHITE;
    bool m_active = true;
};