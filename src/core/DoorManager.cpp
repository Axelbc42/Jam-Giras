#include "DoorManager.h"

DoorManager* DoorManager::s_instance = nullptr;

DoorManager* DoorManager::Instance()
{
	if (s_instance == nullptr) {
		s_instance = new DoorManager();
	}
	return s_instance;
}

void DoorManager::Init()
{
	for (int i = 0; i < DOOR_INIT_POOL; i++) {
		CreateDoorWithKey();
	}
}

void DoorManager::Release()
{
	if (s_instance == nullptr)
		return;

	s_instance->Unload();

	delete s_instance;
	s_instance = nullptr;
}

void DoorManager::Unload()
{
	for (auto k : keys) delete k;
	for (auto d : doors) delete d;
	keys.clear();
	doors.clear();
}

void DoorManager::CreateDoorWithKey()
{
	Door* d = new Door();
	Key* k = new Key(d);

	keys.push_back(k);
	doors.push_back(d);
}

Key* DoorManager::GetKey(int index) 
{ 
	while (keys.size() - 1 < index) {
		CreateDoorWithKey();
	}

	return keys[index]; 
}

Door* DoorManager::GetDoor(int index)
{
	while (doors.size() - 1 < index) {
		CreateDoorWithKey();
	}

	return doors[index];
}


