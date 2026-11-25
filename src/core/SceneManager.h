#pragma once
#include "../scenes/Scene.h"

#include <unordered_map>

class SceneManager
{

public:
	static SceneManager* Instance();
	static void Release();

	void AddScene(Scene* scene);
	void ChangeToLastScene();
	void ChangeScene(int id);
	void ChangeScene(std::string name);
	Scene* GetScene(std::string name);

	void Draw();
	void DrawDebugUI();
	void Update(float dt);

	void ChangePauseMenu() { pauseMenu = !pauseMenu; }
	bool IsFading() { return fading; }

private:
	SceneManager() = default;
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	void Unload();

	std::unordered_map<int, Scene*> scenes;
	std::unordered_map<std::string, int> ids;

	int currentScene = 0;
	int lastScene = 0;

	bool pauseMenu = false;

	bool fading = false;
	float timer = 0;
	const float FADING_TIME = 2;

	static SceneManager* s_instance;
};

