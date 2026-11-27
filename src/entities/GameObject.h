#pragma once

#include "raylib.h"
#include <math.h>

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

		//Si no se define pivote, que utilice el centro del rect por default
		const Vector2 pivote = has_pivot ? (m_pivot) : (Vector2(m_rect.width / 2.0f, m_rect.height / 2.0f));

		DrawTexturePro(m_texture, origen, destino, pivote, m_rotation, m_tint);
	}

	bool IsActive() const { return m_active; }
	void SetActive(bool active) { m_active = active; }

	const Rectangle& GetRect() const { return m_rect; }
	float getRotation() const { return m_rotation; }
	void SetRect(const Rectangle& rect) { m_rect = rect; }
	void SetPosition(float x, float y) { m_rect.x = x; m_rect.y = y; }
	void SetSize(float w, float h) { m_rect.width = w; m_rect.height = h; }
	void SetRotation(float degrees) { m_rotation = degrees; }

	void SetRotationOverload(float degrees) {
		m_rotation = fmodf(m_rotation + degrees, 360.0f);
		if (m_rotation < 0.0f) m_rotation += 360.0f;
	}

	void SetPivot(float x, float y) { m_pivot.x = x; m_pivot.y = y; has_pivot = true; }

	// Textura + posición + tamaño de golpe
	void Configure(const Texture2D& texture, float x, float y, float w, float h)
	{
		m_texture = texture;
		m_rect = Rectangle{ x, y, w, h };
	}

	// Para las cosas que rotan
	void Configure(const Texture2D& texture, float x, float y, float w, float h,
		float pivotX, float pivotY)
	{
		Configure(texture, x, y, w, h);
		SetPivot(pivotX, pivotY);
	}

	void SetTexture(const Texture& texture) { m_texture = texture; }
	Texture2D GetTexture() const { return m_texture; }
	void SetTint(const Color& tint) { m_tint = tint; }


protected:
	Rectangle m_rect{};
	float m_rotation = 0.0f;
	Texture2D m_texture{};
	Color m_tint = WHITE;
	bool m_active = true;
	Vector2 m_pivot{};
	bool has_pivot = false;
};