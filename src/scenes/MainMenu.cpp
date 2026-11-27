#include "MainMenu.h"

#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../utilities/GameConstants.h"
#include "../Game.h"

#include "imgui.h"

void MainMenu::Init()
{
	clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
	TraceLog(LOG_INFO, "Sound loaded: %d", clickSound.frameCount);

	_background = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_MENU);
	m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);
	_backgroundHS = { 0, 0, GameConstants::kScreenWidth, GameConstants::kScreenHeight };

	Texture2D btnTex = AssetManager::Instance()->GetTexture(Textures::BUTON1);

	const float btnWidth = 360.f;
	const float btnHeight = 160.f;
	const float centerX = (GameConstants::kScreenWidth - btnWidth) / 2.f;
	const float centerY = (GameConstants::kScreenHeight - btnHeight) / 2.f;

	//Copio el sonido por que no se si va a romper la referencia del lamba al cambiar de escena
	//Si esto es seguro lo suyo seria replicar la logica en los botones con logica parecida
	Sound sonidoClick = this->clickSound;
	Button* bJugar = Create<Button>("JUGAR", Rectangle{ centerX, centerY, btnWidth, btnHeight }, [sonidoClick] {
		AudioManager::Instance()->PlaySound(sonidoClick);
		SceneManager::Instance()->ChangeScene("Plaza");
		}, btnTex);

	// solo texto (el boton es del tamaño del texto para que sea mas representativo)
	const char* txtSalir = "SALIR";
	const float size = 24;
	Vector2 dimText = MeasureTextEx(m_font, txtSalir, size, 2.f);
	const float tCenterX = (GameConstants::kScreenWidth - dimText.x) / 2.f;
	const float tCenterY = (GameConstants::kScreenHeight - dimText.y) / 2.f;
	Create<Button>(
		"SALIR",
		Rectangle{ tCenterX, tCenterY + tCenterY / 2.f, dimText.x, dimText.y },
		[] { Game::Instance()->RequestQuit(); },
		Texture2D{},   // sin textura
		RED,
		m_font);
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