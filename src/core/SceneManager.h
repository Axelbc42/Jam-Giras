#pragma once
#include "../scenes/Scene.h"

#include <vector>

class SceneManager
{

public:
	void AddScene();
	void ChangeScene();
private:
	std::vector<Scene*> scenes;
};

