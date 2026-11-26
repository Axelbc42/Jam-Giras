#pragma once
#include "../scenes/Scene.h"

#include <unordered_map>

class SceneManager
{
public:
	enum class ScheduleState {
		DAY = 0,
		NIGHT
	};
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

	void ChangeUIActive() { UIActive = !UIActive; }
	bool IsFading() const { return fading; }
	std::string GetCurrenSceneName();
	std::string GetLastSceneName();

	void StartScene(const std::string& name); //Para no agregar logica extra sobre el fader y el caso del menu etc

	bool onDayState()const { return _scheduleState == ScheduleState::DAY; }
	bool onNightState()const { return _scheduleState != ScheduleState::DAY; }

	void changeSchedule(const ScheduleState& s) { _scheduleState = s;; }
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

	bool UIActive = false;

	bool fading = false;
	float timer = 0.f;
	const float FADING_TIME = 1.5f;

	static SceneManager* s_instance;
	ScheduleState _scheduleState;
};

