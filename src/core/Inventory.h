#pragma once
#include "../scenes/Scene.h"
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

	void PlaceItem(int pos);

	const float OFFSET = 20;
	const int SEPARATION = 100;
	const float TAM = 60;
	const int MAX_SIZE = 6;

	std::vector<Object> _inventory;
	std::vector<GameObject> _casillas;

	Texture _fondoCasilla;
};
