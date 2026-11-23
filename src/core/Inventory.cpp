#include "Inventory.h"

#include "AssetManager.h"
#include "../utilities/GameConstants.h"

Inventory* Inventory::s_instance = nullptr;

Inventory* Inventory::Instance()
{
	if (s_instance == nullptr) {
		s_instance = new Inventory();
	}
	return s_instance;
}

void Inventory::Init()
{

}

void Inventory::Release()
{
	if (s_instance == nullptr)
		return;

	s_instance->Unload();

	delete s_instance;
	s_instance = nullptr;
}

void Inventory::Unload()
{
	for (auto elem : _inventory) {
		delete elem.item;
		elem.item = nullptr;
	}
	_inventory.clear();

}

void Inventory::Draw()
{
	for (auto& elem : _inventory)
		elem.item->Draw();
}

void Inventory::Update(float dt)
{
	for (auto& elem : _inventory)
		elem.item->Update(dt);
}

void Inventory::AddObject(Interactuable* item, std::string info) {
	_inventory.push_back({ item , info });
}
