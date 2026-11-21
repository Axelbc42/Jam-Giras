#pragma once
#include "GameObject.h"

class Interactuable : public GameObject
{
public:
	bool Contains(const Vector2& point) const {
		return m_active && CheckCollisionPointRec(point, m_rect);
	}
};
