#include "NavigationManager.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "../utilities/GameConstants.h"

NavigationManager* NavigationManager::s_instance = nullptr;

NavigationManager* NavigationManager::Instance()
{
	if (s_instance == nullptr) {
        s_instance = new NavigationManager();
	}
    return s_instance;
}

void NavigationManager::Init()
{
	Texture arrowText = AssetManager::Instance()->GetTexture(Textures::WHITE_ARROW);

	const float W = (float)GameConstants::kScreenWidth;
	const float H = (float)GameConstants::kScreenHeight;
	const float size = 80.0f; 
	const float margin = 20.0f;    

	auto make = [&](auto id, float x, float y, float rotation) {
		Clickable* a = new Clickable();
		a->SetTexture(arrowText);
		a->SetRect(Rectangle{ x, y, size, size });
		a->SetRotation(rotation);
		a->SetActive(false);       
		arrows[id] = a;
		};

	make(Arrow::UP, (W - size) / 2.0f, margin, 0.0f);
	make(Arrow::DOWN, (W - size) / 2.0f, H - size - margin, 180.0f);
	make(Arrow::LEFT, margin, (H - size) / 2.0f, 270.0f);
	make(Arrow::RIGHT, W - size - margin, (H - size) / 2.0f, 90.0f);
}

void NavigationManager::Release()
{
    if (s_instance == nullptr)
        return;

	s_instance->Unload();

    delete s_instance;
    s_instance = nullptr;
}

void NavigationManager::Unload()
{
	for (auto a : arrows) {
		delete a;
	}
}

void NavigationManager::Draw()
{
	for (auto a : arrows) {
		a->Draw();
	}
}

void NavigationManager::Update(float dt)
{
	for (Clickable* a : arrows)
		a->Update(dt);

	if (IsKeyPressed(KEY_LEFT)) {
		arrows[Arrow::LEFT]->OnClick();
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		arrows[Arrow::RIGHT]->OnClick();
	}
	
	if (IsKeyPressed(KEY_UP)) {
		arrows[Arrow::UP]->OnClick();
	}

	if (IsKeyPressed(KEY_DOWN)) {
		arrows[Arrow::DOWN]->OnClick();
	}
}

void NavigationManager::AddAdjacents(std::string s, SceneAdjacentName a)
{
	SceneManager* sm = SceneManager::Instance();

	auto find = [sm](const std::string& name) -> Scene* {
		return name.empty() ? nullptr : sm->GetScene(name);
		};

	Scene* ns = sm->GetScene(s);

	SceneAdjacent na{
		.up = find(a.up),
		.down = find(a.down),
		.left = find(a.left),
		.right = find(a.right)
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
	arrow->AddCallback([target]() {
		SceneManager::Instance()->ChangeScene(target->GetName());
	});
}