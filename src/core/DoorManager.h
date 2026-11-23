#pragma once
#include "../entities/items/door.h"
#include "../entities/items/key.h"
#include <vector>

constexpr int DOOR_INIT_POOL = 3;

class DoorManager
{
public:
	static DoorManager* Instance();
	static void Release();

	void Init();
	void Unload();

	void CreateDoorWithKey();

	Key* GetKey(int index);
	Door* GetDoor(int index);

private:
	DoorManager() = default;
	~DoorManager() = default;

	DoorManager(const DoorManager&) = delete;
	DoorManager& operator=(const DoorManager&) = delete;

	std::vector<Key*> keys;
	std::vector<Door*> doors;

	static DoorManager* s_instance;
};

