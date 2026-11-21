#include "Draggable.h"

void Draggable::Update(float dt)
{
	Interactuable::Update(dt); 

	if (!m_active){
		m_dragging = false;
		return;
	}

	const Vector2 mouse = GetMousePosition();

	if (!m_dragging)
	{
		if (m_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			m_dragging = true;
			m_startPos = Vector2{ m_rect.x, m_rect.y };
			m_grabOffset = Vector2{ mouse.x - m_rect.x, mouse.y - m_rect.y };
		}
		return;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		float x = mouse.x - m_grabOffset.x;
		float y = mouse.y - m_grabOffset.y;

		const float maxX = (float)GetScreenWidth() - m_rect.width;
		const float maxY = (float)GetScreenHeight() - m_rect.height;
		if (x < 0.0f) x = 0.0f;
		if (y < 0.0f) y = 0.0f;
		if (x > maxX) x = maxX;
		if (y > maxY) y = maxY;

		SetPosition(x, y);
		return;
	}

	m_dragging = false;
	if (m_onDrop)
		m_onDrop();
}