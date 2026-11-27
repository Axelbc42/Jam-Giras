#include "PauseMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"
#include "../utilities/OptionsPanel.h"
#include "../Game.h"
#include "../entities/Button.h"

#include "imgui.h"

void PauseMenu::Init()
{
	_font = AssetManager::Instance()->GetFont(Fonts::MAIN);
	Sound click = AssetManager::Instance()->GetSound(Sounds::CLICK);
	_inkP = { 40, 30, 60, 255 };
	float size = 34.f;

	// Panel: x=470, y=150, 340x420
	Vector2 d = MeasureTextEx(_font, "REANUDAR", size, 2.f);
	Create<Button>("REANUDAR", Rectangle{ 470 + (340 - d.x) / 2.f, 150 + 290, d.x, d.y },
		[click] {
			AudioManager::Instance()->PlaySound(click);
			SceneManager::Instance()->ChangeToLastScene();
			SceneManager::Instance()->ChangeUIActive();
		}, Texture2D{}, _inkP, _font);

	d = MeasureTextEx(_font, "MENÚ", size, 2.f);
	Create<Button>("MENÚ", Rectangle{ 470 + (340 - d.x) / 2.f, 150 + 345, d.x, d.y },
		[click] {
			AudioManager::Instance()->PlaySound(click);
			SceneManager::Instance()->ChangeScene("Menu");
			SceneManager::Instance()->ChangeUIActive();
		}, Texture2D{}, _inkP, _font);
}

void PauseMenu::Draw()
{

	DrawRectangle(0, 0, 1280, 720, Color{ 250, 248, 242, 170 });
	DrawRectangle(470, 150, 340, 420, Color{ 245, 241, 232, 255 });
	DrawRectangleLinesEx(Rectangle{ 470, 150, 340, 420 }, 2.f, _inkP);

	Vector2 t = MeasureTextEx(_font, "PAUSA", 44.f, 2.f);
	float tx = 470 + (340 - t.x) / 2.f;
	DrawTextEx(_font, "PAUSA", Vector2{ tx, 174 }, 44.f, 2.f, _inkP);

	Scene::Draw();

	OptionsPanel::Draw(500, 250, 280, 170);
}