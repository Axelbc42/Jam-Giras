#include "Bar.h"
#include "../core/AssetManager.h"

void Bar::Init()
{
	_clock = Create<GameObject>();
	_bigHand = Create<GameObject>();
	_smallHand = Create<GameObject>();

	// creado en el centro y ya cuando este la escena dibujada con el reloj se traslada y escala al lugar
	const float centerY = GameConstants::kScreenHeight / 2.f;
	const float centerX = GameConstants::kScreenWidth / 2.f;

	auto* asset = AssetManager::Instance();
	const float wClock = 140.f;
	const float hClock = 140.f;
	const float wBigHand = 10.f;
	const float hBigHand = 45.f;
	const float wSMHand = 7.f;
	const float hSMHand = 20.f;

	_clock->Configure(asset->GetTexture(Textures::CLOCK), centerX - (wClock / 2.f), centerY - (hClock / 2.f), wClock, hClock);
	_bigHand->Configure(asset->GetTexture(Textures::HAND), centerX - (wBigHand / 2.f), centerY - (hBigHand / 2.f), wBigHand, hBigHand, wBigHand / 2.f, hBigHand); //tiene rotacion
	_smallHand->Configure(asset->GetTexture(Textures::HAND), centerX - (wSMHand / 2.f), centerY - (hSMHand / 2.f), wSMHand, hSMHand, wSMHand / 2.f, hSMHand);

	//Botones por encima
}

void Bar::Update(float dt)
{

}

void Bar::Draw()
{
	Scene::Draw();

}

void Bar::Release()
{

}

void Bar::DrawDebugUI()
{
	ImGui::PushID(0);
	ImGui::Separator();
	ImGui::LabelText("Manecilla Grande", "");
	if (ImGui::Button("-")) {
		_bigHand->SetRotationOverload(-30.f);
	}
	if (ImGui::Button("+")) {
		_bigHand->SetRotationOverload(30.f);
	}
	ImGui::Separator();
	ImGui::PopID();

	ImGui::PushID(1);
	ImGui::LabelText("Manecilla Pequena", "");
	if (ImGui::Button("-")) {
		_smallHand->SetRotationOverload(-30.f);
	}
	if (ImGui::Button("+")) {
		_smallHand->SetRotationOverload(30.f);
	}
	ImGui::Separator();
	ImGui::PopID();
}