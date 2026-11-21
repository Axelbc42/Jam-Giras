#include "NavigationManager.h"
#include "SceneManager.h"

NavigationManager* NavigationManager::s_instance = nullptr;

NavigationManager* NavigationManager::Instance()
{
	if (s_instance == nullptr) {
        s_instance = new NavigationManager();
	}
    return s_instance;
}

void NavigationManager::Release()
{
    if (s_instance == nullptr)
        return;

    delete s_instance;
    s_instance = nullptr;
}

void NavigationManager::AddAdjacents(std::string s, SceneAdjacentName a)
{
	Scene* ns = SceneManager::Instance()->GetScene(s);
	SceneAdjacent na{
		.up = SceneManager::Instance()->GetScene(a.up),
		.down = SceneManager::Instance()->GetScene(a.down),
		.left = SceneManager::Instance()->GetScene(a.left),
		.right = SceneManager::Instance()->GetScene(a.right)
	};
	AddAdjacents(ns, na);
}

void NavigationManager::UpdateArrows(Scene* currentScene)
{
	SceneAdjacent adj;
	auto it = map.find(currentScene);
	if (it != map.end())
		adj = it->second;

	SetupArrow(Arrow::UP, adj.up);
	SetupArrow(Arrow::DOWN, adj.down);
	SetupArrow(Arrow::LEFT, adj.left);
	SetupArrow(Arrow::RIGHT, adj.right);
}

void NavigationManager::SetupArrow(Arrow id, Scene* target)
{
	auto* arrow = arrows[id];
	if (arrow == nullptr)
		return;

	if (target == nullptr)
	{
		arrow->SetActive(false);
		return;
	}

	arrow->SetActive(true);
	arrow->ResetCallbacks();
	arrow->AddCallBack([target]() {
		SceneManager::Instance()->ChangeScene(target->GetName());
	});
}