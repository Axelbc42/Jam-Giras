#pragma once
#include "../scenes/Scene.h"

#include <unordered_map>

class SceneManager
{

public:
	static SceneManager* Instance();
	static void Release();

	void AddScene(Scene* scene);
	void ChangeScene(int id);
	void ChangeScene(std::string name);
	Scene* GetScene(std::string name);

	void Draw();
	void DrawDebugUI();
	void Update(float dt);

private:
	SceneManager() = default;
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	void Unload();

	std::unordered_map<int, Scene*> scenes;
	std::unordered_map<std::string, int> ids;

	int currentScene = 0;

	static SceneManager* s_instance;
};

