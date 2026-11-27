#pragma once
#include "Scene.h"
#include "raylib.h"
class Bar : public Scene
{
public:
	Bar(std::string name) : Scene(name) {}
	void Init() override;
	void Update(float dt) override;
	void Draw() override;
	void DrawDebugUI() override;
	void Release() override;

	void OnEnter() override;
	void OnExit() override;

private:
	GameObject* _clock;
	GameObject* _bigHand;
	GameObject* _smallHand;
	Sound _handsTickSFX{};
};


//struct Sprite {
//	Texture2D _tex{};
//	Rectangle _sourceRec{};
//	Rectangle _destRec{};
//	Vector2 _pivote{};
//	float _rotacion = 0.f;

//};