#include "Inventory.h"

#include "../utilities/GameConstants.h"
#include "../core/AssetManager.h"

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
	_fondoCasilla = AssetManager::Instance()->GetTexture(Textures::BUTON1);
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
	//for (auto elem : _inventory) {
	//	delete elem.item;
	//	elem.item = nullptr;
	//}
	_inventory.clear();
}

void Inventory::Draw()
{
	for (int i = 0; i < _inventory.size(); i++) {
		float posX = i * SEPARATION + OFFSET;
		Rectangle src{ 0.0f, 0.0f, _fondoCasilla.width, _fondoCasilla.height };
		Rectangle dst{ posX, OFFSET, TAM + OFFSET, TAM + OFFSET };
		DrawTexturePro(_fondoCasilla, src, dst, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);

		_inventory[i].item->Draw();
	}
}

void Inventory::Update(float dt)
{
	for (auto& elem : _inventory)
		elem.item->Update(dt);
}

void Inventory::AddObject(Interactuable* item, std::string info) {
	_inventory.push_back({ item , info });
	item = nullptr;
	float posX = (_inventory.size() - 1) * SEPARATION + OFFSET;
	_inventory.back().item->SetRect(Rectangle{ posX + OFFSET / 2, OFFSET + OFFSET / 2, TAM, TAM });
}
