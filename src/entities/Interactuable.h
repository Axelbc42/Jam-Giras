#pragma once
#include "GameObject.h"
#include "imgui.h"

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
	}

protected:
	bool m_hovered = false;

};
