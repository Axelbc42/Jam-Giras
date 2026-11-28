#pragma once
#include "Scene.h"
class Creditos :
	public Scene
{
public:
	Creditos(std::string name) : Scene(name) {}
	void Init() override;
	void Draw() override;
	void Update(float dt) override;
private:
	Texture2D _background{};
	Rectangle _backgroundHS{};
	Sound _clickSound{};
	Font m_font{};
};

