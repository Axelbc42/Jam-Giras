#include "Game.h"

#include "imgui.h"
#include "rlImGui.h"
#include "core/AssetManager.h"
#include "core/AudioManager.h"
#include "core/SceneManager.h"
#include "core/DialogueManager.h"
#include "core/NavigationManager.h"
#include "utilities/GameConstants.h"

#include "scenes/MainMenu.h"
#include "scenes/PlazaScene.h"
#include "scenes/SceneTextTest.h"
#include "scenes/PauseMenu.h"

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
    InitWindow(GameConstants::kScreenWidth, GameConstants::kScreenHeight, GameConstants::kTitle);
    if (!IsWindowReady())
        return false;

	AudioManager::Instance()->Init();
	SetTargetFPS(60);
	rlImGuiSetup(true);
	m_windowReady = true;
	return true;
}

void Game::initGame()
{
    SceneManager::Instance();
	NavigationManager::Instance()->Init();

    SceneManager::Instance()->AddScene(new MainMenu("Menu"));
	SceneManager::Instance()->AddScene(new PlazaScene("Plaza"));
	SceneManager::Instance()->AddScene(new SceneTextTest("TextTest"));
	SceneManager::Instance()->AddScene(new PauseMenu("PauseMenu"));

	NavigationManager::Instance()->AddAdjacents("Menu", { "Plaza", "TextTest", "", "" });
	NavigationManager::Instance()->AddAdjacents("Plaza", { "Menu", "TextTest", "", "" });
	NavigationManager::Instance()->AddAdjacents("TextTest", { "", "Plaza", "Menu", "" });
	NavigationManager::Instance()->AddAdjacents("PauseMenu", { "", "", "", "" });

	SceneManager::Instance()->ChangeScene("TextTest");
}
// ---------------------------------------------------------------
// Bucle principal
// ---------------------------------------------------------------
void Game::start()
{
	while (!WindowShouldClose())
	{
		update(GetFrameTime());
		draw();
	}
}

void Game::update(float dt)
{
	if (IsKeyPressed(KEY_F1))
		m_debug = !m_debug;		

    SceneManager::Instance()->Update(dt);
	NavigationManager::Instance()->Update(dt);
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(Color{ 40, 44, 52, 255 });

    // --- Mundo del juego ---
	SceneManager::Instance()->Draw();
	NavigationManager::Instance()->Draw();

    // --- Depuracion con ImGui ---
    if (m_debug)
    {
        rlImGuiBegin();
        drawDebugUI();
        rlImGuiEnd();
    }

    EndDrawing();
}

void Game::drawDebugUI()
{
	ImGui::Begin("Depuracion");
	ImGui::Text("FPS: %d", GetFPS());

	const Vector2 mouse = GetMousePosition();
	ImGui::Text("Raton: %.0f, %.0f", mouse.x, mouse.y);

    SceneManager::Instance()->DrawDebugUI();

	ImGui::Separator();
	ImGui::Checkbox("Demo de ImGui", &m_showImGuiDemo);
	ImGui::End();

	if (m_showImGuiDemo)
		ImGui::ShowDemoWindow(&m_showImGuiDemo);
}