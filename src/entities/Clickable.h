#pragma once

#include <functional>
#include <utility>
#include <vector>
#include "Interactuable.h"
#include "GameObject.h"

class Clickable : public Interactuable
{
public:
	void Update(float dt) override;

	bool IsHovered() const { return m_hovered; }

	void AddCallback(std::function<void()> f){
		m_callbacks.push_back(std::move(f));
	}

	void ResetCallbacks(){
		m_callbacks.clear();
	}

	virtual void OnClick(){
		if (!m_active)
			return;

		const std::vector<std::function<void()>> copia = m_callbacks;
		for (const auto& f : copia){
			if (f) f();
		}
	}

private:
	std::vector<std::function<void()>> m_callbacks;
	bool m_hovered = false;
};