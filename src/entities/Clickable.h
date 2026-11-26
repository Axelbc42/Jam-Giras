#pragma once

#include <functional>
#include <utility>
#include <vector>
#include "Interactuable.h"
#include "GameObject.h"
#include <string>
#include "../core/DialogueManager.h"

class Clickable : public Interactuable
{
public:
	~Clickable() override = default;

	void Update(float dt) override{
		Interactuable::Update(dt);

		if (m_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			OnClick();
	}

	void AddCallback(std::function<void()> f){
		m_callbacks.push_back(std::move(f));
	}

	void ResetCallbacks(){
		m_callbacks.clear();
	}

	void AddClickDialogue(const std::string& assetName) {
		AddCallback([this, assetName]() {
			DialogueManager::Instance()->OpenText(assetName);
			});
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
};