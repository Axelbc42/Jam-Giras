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
#include "utilities/GameConstants.h"

#include "scenes/MainMenu.h"
#include "scenes/PlazaScene.h"
#include "scenes/PlazaClickableScene.h"
#include "scenes/SceneTextTest.h"
#include "scenes/PauseMenu.h"

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
	SceneManager::Instance()->AddScene(new PlazaClickableScene("PlazaClickable"));
	SceneManager::Instance()->AddScene(new SceneTextTest("TextTest"));
	SceneManager::Instance()->AddScene(new PauseMenu("PauseMenu"));

	NavigationManager::Instance()->AddAdjacents("Menu", { "Plaza", "TextTest", "", "" });
	NavigationManager::Instance()->AddAdjacents("Plaza", { "Menu", "TextTest", "PlazaClickable", "" });
	NavigationManager::Instance()->AddAdjacents("PlazaClickable", { "", "TextTest", "", "Plaza" });
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

	CursorState::BeginFrame();                    
	DialogueManager::Instance()->Update(dt);
    SceneManager::Instance()->Update(dt);
	NavigationManager::Instance()->Update(dt);
	Inventory::Instance()->Update(dt);

	CursorState::Apply();                         
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(Color{ 40, 44, 52, 255 });

	// Se inicia aqui para poder usar Imgui en las escenas
	rlImGuiBegin();

    // --- Mundo del juego ---
  	SceneManager::Instance()->Draw();
	NavigationManager::Instance()->Draw();
	DialogueManager::Instance()->Draw();
	Inventory::Instance()->Draw();

    // --- Depuracion con ImGui ---
    if (m_debug)
        drawDebugUI();

	rlImGuiEnd();
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


	//DialogueManager
	ImGui::Separator();
	DialogueManager::Instance()->DrawDebugUI();

	ImGui::End();
	if (m_showImGuiDemo)
		ImGui::ShowDemoWindow(&m_showImGuiDemo);
}