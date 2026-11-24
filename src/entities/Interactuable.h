#pragma once

#include <functional>
#include <utility>
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

	void SetOnHover(std::function<void()> f) { m_onHover = std::move(f); }
	void SetOnUnhover(std::function<void()> f) { m_onUnhover = std::move(f); }

	void SetHoveredColor(Color c) { m_hoveredColor = c; if (m_hovered) SetTint(c); }
	void SetNormalColor(Color c) { m_normalColor = c;  if (!m_hovered) SetTint(c); }

	bool IsHovered() const { return m_hovered; }

	void ClearHover() { SetHovered(false); }

	void Update(float dt) override {
		GameObject::Update(dt);

		const bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;
		SetHovered(!imguiCapturaRaton && Contains(GetMousePosition()));

		if (m_hovered)
			CursorState::RequestHand();
	}

protected:
	bool m_hovered = false;
	Color m_normalColor = WHITE;
	Color m_hoveredColor = LIGHTGRAY;
	std::function<void()> m_onHover;
	std::function<void()> m_onUnhover;

private:
	void SetHovered(bool hovered) {
		if (hovered == m_hovered)
			return;

		m_hovered = hovered;
		SetTint(hovered ? m_hoveredColor : m_normalColor);

		if (hovered) { if (m_onHover)   m_onHover(); }
		else { if (m_onUnhover) m_onUnhover(); }
	}
};