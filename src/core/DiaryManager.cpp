#include "DiaryManager.h"
#include "SceneManager.h"

DiaryManager* DiaryManager::s_instance = nullptr;

DiaryManager* DiaryManager::Instance()
{
	if (s_instance == nullptr) {
		s_instance = new DiaryManager();
	}
	return s_instance;
}

void DiaryManager::Release()
{
	if (s_instance == nullptr)
		return;

	s_instance->Unload();

	delete s_instance;
	s_instance = nullptr;
}

void DiaryManager::Init()
{
	_diary = static_cast<Diary*>(SceneManager::Instance()->GetScene("Diary"));
	_nodes.resize(_diary->GetNodesSize(), false);
}

void DiaryManager::Unload()
{
	_diary = nullptr;
	_nodes.clear();
}

void DiaryManager::SetActiveNode(int id, bool b) {
	_diary->SetActiveNode(id, b);
	_nodes[id] = b;
}

bool DiaryManager::IsDiaryCompleted()
{
	for (bool b : _nodes) {
		if (!b) 
			return false;
	}
	return true;
}
