#include "Clickable.h"

#include "imgui.h"   

void Clickable::Update(float dt)
{
	GameObject::Update(dt);

	const bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;

	m_hovered = !imguiCapturaRaton && Contains(GetMousePosition());

	if (m_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		OnClick();
}