#include "MainMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"
#include "../entities/Button.h"
#include "../Game.h"
#include "../utilities/OptionsPanel.h"

#include "imgui.h"

void MainMenu::Init()
{
	_clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
	_background = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_MENU);
	m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);
	_backgroundHS = { 0, 0, GameConstants::kScreenWidth, GameConstants::kScreenHeight };

	_inkP = { 40, 30, 60, 255 };
	float size = 34.f;
	Sound click = _clickSound;
	float miniOffsetX = 30.0f;

	// Botones del menu
	Vector2 d;
	d = MeasureTextEx(m_font, "JUGAR", size, 2.f);
	_bJugar = Create<Button>("JUGAR", Rectangle{ 1190 - d.x, 440, d.x, d.y },
		[click] {
			AudioManager::Instance()->PlaySound(click);
			SceneManager::Instance()->ChangeScene("Plaza");
		}, Texture2D{}, _inkP, m_font, DARKGREEN);

	d = MeasureTextEx(m_font, "OPCIONES", size, 2.f);
	_bOpciones = Create<Button>("OPCIONES", Rectangle{ miniOffsetX + 1190 - d.x, 495, d.x, d.y },
		[this, click] {
			AudioManager::Instance()->PlaySound(click);
			_enOpciones = true;
			SetBotones();
		}, Texture2D{}, _inkP, m_font, DARKPURPLE);

	d = MeasureTextEx(m_font, "CRÉDITOS", size, 2.f);
	_bCreditos = Create<Button>("CRÉDITOS", Rectangle{ miniOffsetX + 1190 - d.x, 550, d.x, d.y },
		[click] {
			AudioManager::Instance()->PlaySound(click);
			// TODO: creditos
		}, Texture2D{}, _inkP, m_font, YELLOW);

	d = MeasureTextEx(m_font, "SALIR", size, 2.f);
	_bSalir = Create<Button>("SALIR", Rectangle{ 1190 - d.x, 605, d.x, d.y },
		[] { Game::Instance()->RequestQuit(); }, Texture2D{}, _inkP, m_font, RED);

	// Boton de las opciones, centrado bajo el panel
	d = MeasureTextEx(m_font, "VOLVER", size, 2.f);
	_bVolver = Create<Button>("VOLVER", Rectangle{ (1280 - d.x) / 2.f, 190 + 280, d.x, d.y },
		[this, click] {
			AudioManager::Instance()->PlaySound(click);
			_enOpciones = false;
			SetBotones();
		}, Texture2D{}, _inkP, m_font);

	SetBotones();
}

void MainMenu::SetBotones()
{
	_bJugar->SetActive(!_enOpciones);
	_bOpciones->SetActive(!_enOpciones);
	_bCreditos->SetActive(!_enOpciones);
	_bSalir->SetActive(!_enOpciones);
	_bVolver->SetActive(_enOpciones);
}

void MainMenu::Update(float dt)
{
	if (_enOpciones && IsKeyPressed(KEY_ESCAPE)) { //pa que pueda cerrar tambien por ESC
		_enOpciones = false;
		SetBotones();
	}
	Scene::Update(dt);
}

void MainMenu::Draw()
{
	DrawTexturePro(_background, { 0, 0, (float)_background.width, (float)_background.height },
		_backgroundHS, { 0, 0 }, 0.0f, WHITE);

	if (_enOpciones) {
		// velo + panel (340x340 centrado: x=470, y=190)
		DrawRectangle(0, 0, 1280, 720, Color{ 250, 248, 242, 200 });
		DrawRectangle(470, 190, 340, 340, Color{ 245, 241, 232, 255 });
		DrawRectangleLinesEx(Rectangle{ 470, 190, 340, 340 }, 2.f, _inkP);

		Vector2 t = MeasureTextEx(m_font, "OPCIONES", 44.f, 2.f);
		float tx = 470 + (340 - t.x) / 2.f;
		DrawTextEx(m_font, "OPCIONES", Vector2{ tx, 214 }, 44.f, 2.f, _inkP);
	}

	Scene::Draw();

	if (_enOpciones)
		OptionsPanel::Draw(500, 290, 280, 170);
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