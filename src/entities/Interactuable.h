#pragma once
#include "GameObject.h"
#include "imgui.h"
#include "../utilities/CursorState.h"

class Interactuable : public GameObject
{
public:
	virtual ~Interactuable() = default;

	bool Contains(const Vector2& point) const {
		return m_active && CheckCollisionPointRec(point, m_rect);
	}

	void SetHoveredColor(Color c) { m_hoveredColor = c; }
	bool IsHovered() const { return m_hovered; }
	void ClearHover() { m_hovered = false; }

	void Update(float dt) override
	{
		GameObject::Update(dt);

		const bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;
		m_hovered = !imguiCapturaRaton && Contains(GetMousePosition());

		if (m_hovered)
			CursorState::RequestHand();

		SetTint(m_hovered ? m_normalColor : m_hoveredColor);
	}

protected:
	bool m_hovered = false;
	Color m_hoveredColor = LIGHTGRAY;
	Color m_normalColor = m_tint;
};
