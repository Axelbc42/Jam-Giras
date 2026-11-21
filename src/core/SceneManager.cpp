#include "SceneManager.h"
#include "NavigationManager.h"
#include "raylib.h"

SceneManager* SceneManager::s_instance = nullptr;

SceneManager* SceneManager::Instance()
{
    if (s_instance == nullptr)
        s_instance = new SceneManager();
    return s_instance;
}

void SceneManager::Release()
{
    if (s_instance == nullptr)
        return;

    s_instance->Unload();
    delete s_instance;
    s_instance = nullptr;
}

void SceneManager::Unload()
{
    for (auto s : scenes) {
        delete s.second;
    }

    scenes.clear();
}

void SceneManager::Draw()
{
    if (pauseMenu)
        scenes[lastScene]->Draw();
    scenes[currentScene]->Draw();
}

void SceneManager::DrawDebugUI()
{
    scenes[currentScene]->DrawDebugUI();
}

void SceneManager::Update(float dt)
{
    if (IsKeyPressed(KEY_SPACE)) {
        if (!pauseMenu)
            ChangeScene("PauseMenu");
        else
            ChangeToLastScene();
        ChangePauseMenu();
    }

    scenes[currentScene]->Update(dt);
}

void SceneManager::AddScene(Scene* scene)
{
    int id = scenes.size();

    scenes.emplace(id, scene);
    ids.emplace(scene->GetName(), id);

    scene->Init();
}

void SceneManager::ChangeToLastScene() {
    auto it = scenes.find(lastScene);

    if (it == scenes.end()) {
        TraceLog(LOG_INFO, "ESCENA NO ENCONTRADA");
        return;
    }

    int id = lastScene;
    lastScene = currentScene;
    currentScene = id;
    NavigationManager::Instance()->UpdateArrows(scenes[currentScene]);
}

void SceneManager::ChangeScene(int id)
{
    auto it = scenes.find(id);

    if (it == scenes.end()) {
        TraceLog(LOG_INFO, "ESCENA NO ENCONTRADA");
        return;
    }

    lastScene = currentScene;
    currentScene = id;

    NavigationManager::Instance()->UpdateArrows(scenes[currentScene]);
}

void SceneManager::ChangeScene(std::string name)
{
    auto it = ids.find(name);

    if (it == ids.end()) {
        TraceLog(LOG_WARNING, "ESCENA %s NO ENCONTRADA", name.c_str());
        return;
    }

    ChangeScene(it->second);
}

Scene* SceneManager::GetScene(std::string name) {
    auto it = ids.find(name);

    if (it == ids.end()) {
        TraceLog(LOG_WARNING, "ESCENA %s NO ENCONTRADA", name.c_str());
        return nullptr;
    }

    return scenes[it->second];
}