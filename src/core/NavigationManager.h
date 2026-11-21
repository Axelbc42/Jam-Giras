#pragma once
#include <unordered_map>
#include "../scenes/Scene.h"
#include "../entities/Clickable.h"
#include "../entities/GameObject.h"
#include <array>

class NavigationManager
{
public:
	static NavigationManager* Instance();
	static void Release();

	void Init();
	void Unload();
	void Draw();
	void Update(float dt);

	struct SceneAdjacent {
		Scene* up = nullptr;
		Scene* down = nullptr;
		Scene* left = nullptr;
		Scene* right = nullptr;
	};

	struct SceneAdjacentName {
		std::string up = "";
		std::string down = "";
		std::string left = "";
		std::string right = "";
	};

	void AddAdjacents(Scene* s, SceneAdjacent a) {
		map.emplace(s, a);
	}
	void AddAdjacents(std::string s, SceneAdjacentName a);

	enum Arrow { UP, DOWN, LEFT, RIGHT };
	void UpdateArrows(Scene* currentScene);
	void SetupArrow(Arrow id, Scene* target);

private:
	NavigationManager() = default;
	~NavigationManager() = default;

	NavigationManager(const NavigationManager&) = delete;
	NavigationManager& operator=(const NavigationManager&) = delete;

	std::unordered_map<Scene*, SceneAdjacent> map;

	std::array<Clickable*, 4> arrows;

	static NavigationManager* s_instance;
};

