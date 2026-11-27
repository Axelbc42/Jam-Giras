#pragma once
#include "GameObject.h"
#include <iostream>
#include <functional>
#include <imgui.h>

using namespace std;
using BtnCallback = std::function<void()>;

class Button : public GameObject {
public:
	~Button() override = default;
	Button(Button&) = delete;
	Button& operator=(const Button&) = delete;

	Button(const std::string& text, Rectangle rect,
		std::function<void()> onClick, Texture2D bg = {}, Color textC = BLACK, Font f = AssetManager::Instance()->GetFont(Fonts::MAIN)) : GameObject(),
		_text(text), _onClick(move(onClick)), _font(f) {

		m_rect = rect;
		m_texture = bg;
		m_tint = textC;
	}

	void Update(float dt) override {

		bool imguiCapturaRaton = ImGui::GetIO().WantCaptureMouse;
		Vector2 mouse = GetMousePosition();

		_hover = !imguiCapturaRaton && CheckCollisionPointRec(mouse, m_rect); //colorea o no dependiendo
		if (_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			_onClick();
			return; // por si onClick cambia de escena y se invalida algo
		}
	}

	void Draw() override {
		if (m_texture.id != 0) { //Si tiene textura, el hover se refleja en la tex
			Color tinte = _hover ? WHITE : Color{ 200, 200, 200, 255 };
			DrawTexturePro(m_texture, { 0, 0, (float)m_texture.width, (float)m_texture.height },
				m_rect, { 0, 0 }, 0.0f, tinte);
		}

		// texto centrado en el rect
		const float size = 24, spacing = 2;
		Vector2 ts = MeasureTextEx(_font, _text.c_str(), size, spacing);
		Vector2 pos = { m_rect.x + (m_rect.width - ts.x) / 2,
						m_rect.y + (m_rect.height - ts.y) / 2 };
		Color c = _hover ? YELLOW : m_tint; // hover del texto si no tiene fondo con textura
		DrawTextEx(_font, _text.c_str(), pos, size, spacing, c);
	}

private:
	std::string _text;
	BtnCallback _onClick;
	bool _hover = false;
	Font _font{}; //Que se lo pase la escena
};