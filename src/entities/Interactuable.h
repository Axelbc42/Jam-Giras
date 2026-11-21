#pragma once
#include "GameObject.h"
#include "imgui.h"
#include "../utilities/CursorState.h"

class Interactuable : public GameObject
{
public:
	bool Contains(const Vector2& point) const {
		return m_active && CheckCollisionPointRec(point, m_rect);
	}

	bool IsHovered() const { return m_hovered; }

	void Update(float dt) override
	{
		GameObject::Update(dt);

		const bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;
		m_hovered = !imguiCapturaRaton && Contains(GetMousePosition());

		if (m_hovered)
			CursorState::RequestHand();
	}

protected:
	bool m_hovered = false;

};
