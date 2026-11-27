#include "SceneManager.h"
#include "NavigationManager.h"
#include "../utilities/GameConstants.h"
#include "raylib.h"
#include "raymath.h"

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
    if (!fading) {
        if (UIActive)
            scenes[lastScene]->Draw();
        scenes[currentScene]->Draw();
    }
    else {
        if (timer > FADING_TIME / 2.f)
            scenes[lastScene]->Draw();
        else
            scenes[currentScene]->Draw();

        float a = 1.f - fabsf(2.f * timer / FADING_TIME - 1.f);
        unsigned char alpha = a * 255.f;
        DrawRectangle(0, 0, GameConstants::kScreenWidth, GameConstants::kScreenHeight, Color{ 0, 0, 0, alpha });
    }
}

void SceneManager::DrawDebugUI()
{
    scenes[currentScene]->DrawDebugUI();
}

void SceneManager::Update(float dt)
{
    if (!fading) {
        if (IsKeyPressed(KEY_ESCAPE) && GetCurrenSceneName() != "Menu") {
             if (!UIActive)
                ChangeScene("PauseMenu");
            else
                ChangeToLastScene();
            ChangeUIActive();
        }
    }
    else {
        timer -= dt;
        if (timer <= 0)
            fading = false;
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
    if (currentScene == id)
        return;

    auto it = scenes.find(id);

    if (it == scenes.end()) {
        TraceLog(LOG_INFO, "ESCENA NO ENCONTRADA");
        return;
    }

    scenes[currentScene]->OnExit(); //pa cuando haga falta, modificar su ubi si es que no va aqui

    lastScene = currentScene;
    currentScene = id;

    NavigationManager::Instance()->UpdateArrows(scenes[currentScene]);
    scenes[currentScene]->OnEnter();
}

void SceneManager::ChangeScene(std::string name)
{
    auto it = ids.find(name);

    if (it == ids.end()) {
        TraceLog(LOG_WARNING, "ESCENA %s NO ENCONTRADA", name.c_str());
        return;
    }

    ChangeScene(it->second);

    if (name != "PauseMenu" && name != "Diary") {
        fading = true;
        timer = FADING_TIME;
    }
}

Scene* SceneManager::GetScene(std::string name) {
    auto it = ids.find(name);

    if (it == ids.end()) {
        TraceLog(LOG_WARNING, "ESCENA %s NO ENCONTRADA", name.c_str());
        return nullptr;
    }

    return scenes[it->second];
}

void SceneManager::StartScene(const std::string& name)
{
    auto it = ids.find(name);
    if (it == ids.end()) {
        TraceLog(LOG_WARNING, "ESCENA %s NO ENCONTRADA", name.c_str());
        return;
    }

    currentScene = lastScene = it->second;   // no hay escena anterior
    NavigationManager::Instance()->UpdateArrows(scenes[currentScene]);
    scenes[currentScene]->OnEnter();        
    // no hace falta OnExit de nadie

    fading = true;
    timer = FADING_TIME / 2.f;               // solo la mitad: negro -> escena
}

std::string SceneManager::GetCurrenSceneName()
{
    for (const auto& [name, id] : ids)
        if (id == currentScene)
            return name;
    return "";
}

std::string SceneManager::GetLastSceneName()
{
    for (const auto& [name, id] : ids)
        if (id == lastScene)
            return name;
    return "";
}