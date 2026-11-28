#pragma once
#include "GameObject.h"
#include <iostream>
#include <functional>
#include <imgui.h>
#include "../core/AssetManager.h"
#include <algorithm>

using namespace std;
using BtnCallback = std::function<void()>;

class Button : public GameObject {
public:
	~Button() override = default;
	Button(Button&) = delete;
	Button& operator=(const Button&) = delete;

	Button(const std::string& text, Rectangle rect,
		std::function<void()> onClick, Texture2D bg = {}, Color textC = BLACK,
		Font f = AssetManager::Instance()->GetFont(Fonts::MAIN), Color hoverC = DARKPURPLE) : GameObject(),
		_text(text), _onClick(move(onClick)), _font(f), _hoverC(hoverC) {

		m_rect = rect;
		m_texture = bg;
		m_tint = textC;
	}

	void Update(float dt) override {
		if (!m_active) return;
		bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;
		Vector2 mouse = GetMousePosition();

		_hover = !imguiCapturaRaton && CheckCollisionPointRec(mouse, m_rect); //colorea o no dependiendo

		float targetScale = _hover ? 1.1f : 1.0f;
		float speed = 12.0f;
		_scale = _scale + std::clamp(speed * dt, 0.f, 1.0f) * (targetScale - _scale); //lerp normal

		if (_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			_onClick();
			return; // por si onClick cambia de escena y se invalida algo
		}
	}

	void Draw() override {
		if (!m_active) return;

		if (m_texture.id != 0) { //Si tiene textura, el hover se refleja en la tex
			float scaledW = m_rect.width * _scale;
			float scaledH = m_rect.height * _scale;
			Rectangle scaledRect = { m_rect.x - (scaledW - m_rect.width) / 2.0f,m_rect.y - (scaledH - m_rect.height) / 2.0f,
				scaledW,scaledH };

			Color tinte = _hover ? WHITE : Color{ 200, 200, 200, 255 };
			DrawTexturePro(m_texture, { 0, 0, (float)m_texture.width, (float)m_texture.height },
				scaledRect, { 0, 0 }, 0.0f, tinte);
		}

		// texto centrado en el rect
		const float baseSize = 24.0f;
		const float baseSpacing = 2.0f;
		float size = baseSize * _scale;
		float spacing = baseSpacing * _scale;

		Vector2 ts = MeasureTextEx(_font, _text.c_str(), size, spacing);
		Vector2 pos = { m_rect.x + (m_rect.width - ts.x) / 2,
						m_rect.y + (m_rect.height - ts.y) / 2 };
		Color c = _hover ? _hoverC : m_tint; // hover del texto si no tiene fondo con textura
		DrawTextEx(_font, _text.c_str(), pos, size, spacing, c);
	}

	//void SetHoverColor(const Color& color) {_hoverC = color;}
private:
	std::string _text;
	BtnCallback _onClick;
	bool _hover = false;
	Font _font{}; //Que se lo pase la escena
	Color _hoverC{};
	float _scale = 1.0f;
};