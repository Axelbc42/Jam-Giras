#include "MainMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"
#include "../entities/Button.h"
#include "../Game.h"

#include "imgui.h"

void MainMenu::Init()
{
	_clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
	_background = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_MENU);
	m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);
	_backgroundHS = { 0, 0, GameConstants::kScreenWidth, GameConstants::kScreenHeight };

	const float fontSize = 36.f;
	const float spacing = 2.f;
	const float marginRight = 60.f;
	const float marginBottom = 60.f;
	const float lineGap = 18.f;

	struct Entry { const char* text; BtnCallback onClick; };
	Sound click = _clickSound;

	Entry entries[] = {
		{ "   JUGAR",    [click] { AudioManager::Instance()->PlaySound(click);
								SceneManager::Instance()->ChangeScene("Plaza"); } },
		{ "OPCIONES", [click] { AudioManager::Instance()->PlaySound(click);
								SceneManager::Instance()->ChangeScene("PauseMenu");  } },
		{ "CRÉDITOS", [click] { AudioManager::Instance()->PlaySound(click);
	/* TODO: SceneManager::Instance()->ChangeScene("Creditos"); */ } },
		{ "   SALIR",    [] { Game::Instance()->RequestQuit(); } },
	};
	const int n = sizeof(entries) / sizeof(entries[0]);

	// Se colocan de abajo hacia arriba: el ultimo (SALIR) queda al fondo
	float y = GameConstants::kScreenHeight - marginBottom;
	for (int i = n - 1; i >= 0; --i)
	{
		Vector2 dim = MeasureTextEx(m_font, entries[i].text, fontSize, spacing);
		y -= dim.y;
		float x = GameConstants::kScreenWidth - marginRight - dim.x; // alineado a la derecha
		Create<Button>(entries[i].text, Rectangle{ x, y, dim.x, dim.y },
			entries[i].onClick, Texture2D{}, Color{ 40, 30, 60, 255 }, m_font);
		y -= lineGap;
	}
}

void MainMenu::Update(float dt)
{
	Scene::Update(dt);
}

void MainMenu::Draw()
{
	DrawTexturePro(_background, { 0, 0, (float)_background.width, (float)_background.height },
		_backgroundHS, { 0, 0 }, 0.0f, WHITE);
	Scene::Draw();

	// --- Interfaz del juego ---
	//DrawText(m_message.c_str(), _buttonHS.x + 60, _buttonHS.y + 30, 24, BLACK);
	//DrawText("F1: panel de depuracion", 20, GameConstants::kScreenHeight - 50, 24, BLACK);
}

void MainMenu::Release()
{
	// Liberar recursos propios de esta escena
}

void MainMenu::DrawDebugUI()
{
	ImGui::Separator();
	ImGui::Text("Hotspot de prueba");
	ImGui::DragFloat("x", &_backgroundHS.x, 1.0f);
	ImGui::DragFloat("y", &_backgroundHS.y, 1.0f);
	ImGui::DragFloat("ancho", &_backgroundHS.width, 1.0f, 10.0f, 1280.0f);
	ImGui::DragFloat("alto", &_backgroundHS.height, 1.0f, 10.0f, 720.0f);
}