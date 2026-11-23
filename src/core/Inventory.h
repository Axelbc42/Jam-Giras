#pragma once
#include "../scenes/Scene.h"
#include "../entities/Clickable.h"
#include "../entities/Draggable.h"
#include "../entities/Interactuable.h"

#include <vector>

struct Object
{
	Interactuable* item;
	std::string info;
};

class Inventory
{
public:
	static Inventory* Instance();
	static void Release();

	void Init();
	void Unload();
	void Draw();
	void Update(float dt);

	void AddObject(Interactuable* item, std::string info);

private:
	Inventory() = default;
	~Inventory() = default;

	Inventory(const Inventory&) = delete;
	Inventory& operator=(const Inventory&) = delete;

	static Inventory* s_instance;

	std::vector<Object> _inventory;
};
