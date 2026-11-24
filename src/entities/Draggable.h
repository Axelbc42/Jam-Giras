#pragma once

#include <functional>
#include <utility>
#include "Clickable.h"

class Draggable : public Clickable
{
public:
	~Draggable() = default;
	void Update(float dt) override;

	bool IsDragging() const { return m_dragging; }

	// Se llama al soltar el objeto
	void SetOnDrop(std::function<void()> f) { m_onDrop = std::move(f); }

	// Devuelve el objeto a donde estaba cuando empezo el ultimo arrastre
	void ReturnToStart() { SetPosition(m_startPos.x, m_startPos.y); }

private:
	bool m_dragging = false;
	Vector2 m_grabOffset{};   
	Vector2 m_startPos{};    
	std::function<void()> m_onDrop;
};