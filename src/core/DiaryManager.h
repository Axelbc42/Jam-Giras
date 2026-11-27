#pragma once
#include "../scenes/Diary.h"
#include <vector>

class DiaryManager
{
public:
	static DiaryManager* Instance();
	static void Release();

	void Init();
	void Unload();

	void SetActiveNode(int id, bool b);
	bool IsDiaryCompleted();

private:
	DiaryManager() = default;
	~DiaryManager() = default;

	DiaryManager(const DiaryManager&) = delete;
	DiaryManager& operator=(const DiaryManager&) = delete;

	static DiaryManager* s_instance;

	Diary* _diary;
	std::vector<bool> _nodes;
};
