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
	_casillas.clear();
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
	for (int i = 0; i < _inventory.size(); i++) {
		// Si esta inactivo lo elimina del vector
		if (!_inventory[i].item->IsActive()) {
			_inventory[i].item = nullptr;
			// Mueve el resto de elementos hacia delante
			for (int j = i; j + 1 < _inventory.size(); ++j) {
				_inventory[j] = _inventory[j + 1];
				PlaceItem(j);
			}
			// Lo elimina
			_inventory.pop_back();
		}
		else
			_inventory[i].item->Update(dt);
	}
}

void Inventory::AddObject(Interactuable* item, std::string info) {
	_inventory.push_back({ item , info });
	item->AddHoverDialogue(info);
	item = nullptr;
	PlaceItem(_inventory.size() - 1);
}

void Inventory::PlaceItem(int pos) {
	float posX = pos * SEPARATION + OFFSET;
	_inventory[pos].item->SetRect(Rectangle{ posX + OFFSET / 2, OFFSET + OFFSET / 2, TAM, TAM });
}
