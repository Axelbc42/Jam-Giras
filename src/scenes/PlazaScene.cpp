#include "PlazaScene.h"
#include "../core/AssetManager.h"
#include "../utilities/GameConstants.h"
#include "../entities/items/door.h"
#include "../entities/items/key.h"
#include "../core/DoorManager.h"
#include "../core/Inventory.h"
#include "../core/DialogueManager.h"
#include "../core/SceneManager.h"
#include "imgui.h"

void PlazaScene::Init()
{
	AssetManager* assets = AssetManager::Instance();
	DoorManager* dm = DoorManager::Instance();

	if (m_plaza == nullptr)
	{
		m_plaza = Create<GameObject>();
		m_plaza->SetTexture(assets->GetTexture(Textures::BACKGROUND_PLAZA));
		m_plaza->SetRect(Rectangle{ 400.0f, 200.0f, 500.0f, 350.0f });

		Door* d = dm->GetDoor(1);
		d->SetTexture(assets->GetTexture(Textures::CLOSED_DOOR));
		d->SetOpenTex(assets->GetTexture(Textures::OPENED_DOOR));
		d->SetRect(Rectangle{ 590.0f, 270.0f, 120.0f, 180.0f });
		Adopt(d);

		Key* k = dm->GetKey(0);
		k->SetTexture(assets->GetTexture(Textures::ITEM_KEY_1));
		k->SetRect(Rectangle{ 100.0f, 300.0f, 80.0f, 80.0f });
		k->AddCallback([k]() {
			k->ResetCallbacks();
			Inventory::Instance()->AddObject(k, FileName::KEY_INFO);
			});
		Adopt(k);

		Key* k2 = dm->GetKey(1);
		k2->SetTexture(assets->GetTexture(Textures::ITEM_KEY_2));
		k2->SetRect(Rectangle{ 100.0f, 400.0f, 80.0f, 80.0f });
		k2->AddCallback([k2]() {
			k2->ResetCallbacks();
			Inventory::Instance()->AddObject(k2, FileName::KEY_INFO);
			});
		Adopt(k2);
	}

	_diary = Create<Clickable>();
	_diary->SetTexture(AssetManager::Instance()->GetTexture(Textures::CLOSED_DIARY));
	_diary->SetRect(Rectangle{ 1000.0f, 50, 100.0f, 50.0f });

	auto abrirDiario = []() {
		if (SceneManager::Instance()->GetCurrenSceneName() != "Diary" && SceneManager::Instance()->GetCurrenSceneName() != "PauseMenu") {
			SceneManager::Instance()->ChangeUIActive();
			SceneManager::Instance()->ChangeScene("Diary");
		}
		else if (SceneManager::Instance()->GetCurrenSceneName() == "Diary") {
			SceneManager::Instance()->ChangeUIActive();
			SceneManager::Instance()->ChangeToLastScene();
		}
	};

	_diary->AddCallback([this, abrirDiario]() {
		Inventory::Instance()->AddObject(_diary, FileName::DIARY_INFO);
		_diary->ResetCallbacks();
		_diary->AddCallback(abrirDiario);
		});

	Adopt(_diary);


	m_font = assets->GetFont(Fonts::MAIN);

	//Dialogos del tutorial.
	DialogueManager::Instance()->SetNext(FileName::DIALOGUE_BOX_1, FileName::DIALOGUE_BOX_2);
	DialogueManager::Instance()->SetNext(FileName::DIALOGUE_BOX_2, FileName::PLAZA);
}

void PlazaScene::OnEnter()
{
	//Generico--NONE
	if (!tutorialShown)
		pendingTutorial = true;

	//Esto es solo un ejemeplo, en donde el diario solo esta por el dia.
	//Si ya se cogio previamente y es de noche sigue estando en el inventario, se puede mejorar la logica..
	if (SceneManager::Instance()->onDayState()) {

		if (Inventory::Instance()->isItemAtInventory(_diary)) return;

		_diary->SetActive(true);
	}
	else {

		if (Inventory::Instance()->isItemAtInventory(_diary)) return;
		
		_diary->SetActive(false);
	}
}

void PlazaScene::Update(float dt)
{
	Scene::Update(dt);

	if (pendingTutorial && !SceneManager::Instance()->IsFading())
	{
		pendingTutorial = false;
		tutorialShown = true;
		DialogueManager::Instance()->OpenText(FileName::DIALOGUE_BOX_1);
	}
}

void PlazaScene::OnExit()
{
	DialogueManager::Instance()->CloseAll();

	if (SceneManager::Instance()->onDayState()) {

	}
	else {

	}
}

void PlazaScene::Draw()
{
	Scene::Draw();

	const Rectangle& r = m_plaza->GetRect();
	DrawTextEx(m_font, "PLAZA", Vector2{ r.x, r.y - 50 }, 50.0f, 2.0f, RAYWHITE);

	DrawText("F1: panel de depuracion", 20, 20, 20, LIGHTGRAY);

}

void PlazaScene::Release()
{
	//el m_plaza ya lo registra y se encarga el create
}

void PlazaScene::DrawDebugUI()
{
	ImGui::Separator();
	ImGui::Text("Plaza");

	Rectangle r = m_plaza->GetRect();
	ImGui::DragFloat("x", &r.x, 1.0f);
	ImGui::DragFloat("y", &r.y, 1.0f);
	ImGui::DragFloat("ancho", &r.width, 1.0f, 10.0f, 1280.0f);
	ImGui::DragFloat("alto", &r.height, 1.0f, 10.0f, 720.0f);
	m_plaza->SetRect(r);
}