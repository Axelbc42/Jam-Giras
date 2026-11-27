#include "Game.h"

#include "imgui.h"
#include "rlImGui.h"
#include "core/AssetManager.h"
#include "core/AudioManager.h"
#include "core/SceneManager.h"
#include "core/DialogueManager.h"
#include "core/DoorManager.h"
#include "core/NavigationManager.h"
#include "core/Inventory.h"
#include "core/DiaryManager.h"
#include "utilities/GameConstants.h"

#include "scenes/MainMenu.h"
#include "scenes/PlazaScene.h"
#include "scenes/CallejonOeste.h"
#include "scenes/CallejonEste.h"
#include "scenes/CallejonNorte.h"
#include "scenes/PuertaBar.h"
#include "scenes/Bar.h"
#include "scenes/PauseMenu.h"
#include "scenes/Diary.h"
#include "scenes/puzzles/MirrorPuzzle.h"

#include "utilities/CursorState.h"

Game* Game::s_instance = nullptr;

// ---------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------
bool Game::Init()
{
	if (s_instance != nullptr)
		return true;

	s_instance = new Game();
	if (!s_instance->initWindow())
	{
		Release();
		return false;
	}
	return true;
}

Game* Game::Instance()
{
	return s_instance;
}

void Game::Release()
{
	delete s_instance;
	s_instance = nullptr;
}

Game::~Game()
{
	if (m_windowReady)
	{
		AssetManager::Release();   // 1) recursos (necesitan la ventana y el audio abiertos)
		AudioManager::Release();
		SceneManager::Release();
		DialogueManager::Release();
		NavigationManager::Release();
		DoorManager::Release();
		Inventory::Release();
		DiaryManager::Release();
		UnloadRenderTexture(m_target);
		CloseAudioDevice();        // 2) audio
		rlImGuiShutdown();         // 3) ImGui
		CloseWindow();             // 4) ventana
	}
}

// ---------------------------------------------------------------
// Inicializacion
// ---------------------------------------------------------------
bool Game::initWindow()
{
	//---ConfigWindow---
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(GameConstants::kScreenWidth, GameConstants::kScreenHeight, GameConstants::kTitle);
	if (!IsWindowReady())
		return false;
	SetWindowMinSize(GameConstants::kScreenWidth, GameConstants::kScreenHeight);
#ifdef GAME_RELEASE
	ToggleBorderlessWindowed(); //Empieza en fullscreen sin bordes //ESTO PARA RELEASE
#endif
#ifdef GAME_DEBUG
	MaximizeWindow();
#endif


	m_target = LoadRenderTexture(GameConstants::kScreenWidth, GameConstants::kScreenHeight);
	SetTextureFilter(m_target.texture, TEXTURE_FILTER_BILINEAR); //hay que ver que tal este o el point por el tipo de arte
	//-----------------

	AudioManager::Instance()->Init();
	SetTargetFPS(60);
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	rlImGuiSetup(true);
	m_windowReady = true;
	return true;
}

void Game::initGame()
{
	SceneManager::Instance();
	NavigationManager::Instance()->Init();
	DialogueManager::Instance()->Init(); //Cargan los textos etc
	DoorManager::Instance()->Init();
	Inventory::Instance()->Init();

	SceneManager::Instance()->AddScene(new MainMenu("Menu"));
	SceneManager::Instance()->AddScene(new PlazaScene("Plaza"));
	SceneManager::Instance()->AddScene(new CallejonOeste("CallejonOeste"));
	SceneManager::Instance()->AddScene(new CallejonEste("CallejonEste"));
	SceneManager::Instance()->AddScene(new CallejonNorte("CallejonNorte"));
	SceneManager::Instance()->AddScene(new PuertaBar("PuertaBar"));
	SceneManager::Instance()->AddScene(new Bar("Bar"));
	SceneManager::Instance()->AddScene(new PauseMenu("PauseMenu"));
	SceneManager::Instance()->AddScene(new Diary("Diary"));
	SceneManager::Instance()->AddScene(new MirrorPuzzle("MirrorPuzzle"));

	DiaryManager::Instance()->Init();

	NavigationManager::Instance()->AddAdjacents("Menu", { "", "", "", "" });
	NavigationManager::Instance()->AddAdjacents("Plaza", { "CallejonNorte", "PuertaBar", "CallejonOeste", "CallejonEste" });
	NavigationManager::Instance()->AddAdjacents("CallejonOeste", { "", "Plaza", "", "" });
	NavigationManager::Instance()->AddAdjacents("CallejonEste", { "", "Plaza", "", "" });
	NavigationManager::Instance()->AddAdjacents("CallejonNorte", { "", "Plaza", "", "" });
	NavigationManager::Instance()->AddAdjacents("PuertaBar", { "", "Plaza", "", "" });
	NavigationManager::Instance()->AddAdjacents("Bar", { "", "Plaza", "MirrorPuzzle", "" });
	NavigationManager::Instance()->AddAdjacents("MirrorPuzzle", { "", "", "", "Bar" });
	NavigationManager::Instance()->AddAdjacents("PauseMenu", { "", "", "", "" });
	NavigationManager::Instance()->AddAdjacents("Diary", { "", "", "", "" });

	SceneManager::Instance()->StartScene("MirrorPuzzle");
}
// ---------------------------------------------------------------
// Bucle principal
// ---------------------------------------------------------------
void Game::start()
{
	while (!m_quit)
	{
		update(GetFrameTime());
		draw();
	}
}

void Game::update(float dt)
{
	if (IsKeyPressed(KEY_F1))
		m_debug = !m_debug;
	if (IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)))
		ToggleBorderlessWindowed();

	updateViewport(); //antes de que se lea el raton

	CursorState::BeginFrame();
	DialogueManager::Instance()->Update(dt);
	SceneManager::Instance()->Update(dt);
	if (!SceneManager::Instance()->IsFading()) {
		NavigationManager::Instance()->Update(dt);
		Inventory::Instance()->Update(dt);
	}

	CursorState::Apply();
}

void Game::draw() //Imp mantener este orden
{
	//ImGui trabaja con coordenadas de ventana, con el reset funciona estando fullscreen o tipo window
	SetMouseOffset(0, 0);
	SetMouseScale(1.f, 1.f);

	rlImGuiBegin();  // el frame de ImGui existe ya para las escenas

	BeginDrawing();
	ClearBackground(BLACK);

	// El juego, a la textura de 1280x720
	BeginTextureMode(m_target);
	ClearBackground(Color{ 40, 44, 52, 255 });
	BeginMode2D(Camera2D{ {0, 0}, {0, 0}, 0.f, m_renderScale });

	SceneManager::Instance()->Draw();
	if (!SceneManager::Instance()->IsFading()) {
		NavigationManager::Instance()->Draw();
	}
	if (SceneManager::Instance()->GetCurrenSceneName() != "Menu")
		Inventory::Instance()->Draw();

	DialogueManager::Instance()->Draw();
	EndMode2D();
	EndTextureMode();

	// La textura, escalada a la ventana
	DrawTexturePro(m_target.texture,
		{ 0, 0, (float)m_target.texture.width, -(float)m_target.texture.height },
		{ m_offX, m_offY, GameConstants::kScreenWidth * m_scale, GameConstants::kScreenHeight * m_scale },
		{ 0, 0 }, 0.f, WHITE);

#ifdef GAME_DEBUG
	if (m_debug) drawDebugUI();
#endif
	rlImGuiEnd(); //Aqui hace lo que tiene que hacer ImGui, encima de la textura
	EndDrawing();
}

void Game::drawDebugUI()
{
	ImGui::Begin("Depuracion");
	if (ImGui::Button("Cerrar app")) {
		m_quit = true;
	}
	ImGui::Text("FPS: %d", GetFPS());

	const Vector2 m = GetMousePosition();
	ImGui::Text("Raton (ventana): %.0f, %.0f", m.x, m.y);
	ImGui::Text("Raton (juego): %.0f, %.0f", (m.x - m_offX) / m_scale, (m.y - m_offY) / m_scale);

	SceneManager::Instance()->DrawDebugUI();

	ImGui::Separator();
	ImGui::Checkbox("Demo de ImGui", &m_showImGuiDemo);

	ImGui::Separator();
	if (ImGui::Button("Cambiar a dia")) {
		SceneManager::Instance()->changeSchedule(SceneManager::ScheduleState::DAY);
	}
	if (ImGui::Button("Cambiar a noche")) {
		SceneManager::Instance()->changeSchedule(SceneManager::ScheduleState::NIGHT);
	}

	//DialogueManager
	ImGui::Separator();
	DialogueManager::Instance()->DrawDebugUI();

	ImGui::End();
	if (m_showImGuiDemo)
		ImGui::ShowDemoWindow(&m_showImGuiDemo);
}


void Game::updateViewport()
{
	const float W = GameConstants::kScreenWidth, H = GameConstants::kScreenHeight;
	m_scale = fminf(GetScreenWidth() / W, GetScreenHeight() / H);

	float wanted = fminf(3.f, fmaxf(1.f, ceilf(m_scale * 2.f) / 2.f));
	if (wanted != m_renderScale) resizeTarget(wanted);

	m_offX = (GetScreenWidth() - W * m_scale) / 2.f;
	m_offY = (GetScreenHeight() - H * m_scale) / 2.f;
	SetMouseOffset((int)-m_offX, (int)-m_offY);
	SetMouseScale(1.f / m_scale, 1.f / m_scale);
}

void Game::resizeTarget(float s)
{
	UnloadRenderTexture(m_target);
	m_target = LoadRenderTexture((int)(GameConstants::kScreenWidth * s),
		(int)(GameConstants::kScreenHeight * s));
	SetTextureFilter(m_target.texture, TEXTURE_FILTER_BILINEAR);
	m_renderScale = s;
}

Vector2 Game::GameToWindow(float x, float y) const {
	return Vector2(m_offX + x * m_scale, m_offY + y * m_scale);
}
