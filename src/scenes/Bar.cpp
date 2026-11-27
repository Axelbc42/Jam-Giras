#include "Bar.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/SceneManager.h"
#include "../entities/Button.h"

void Bar::Init()
{
	//---Sonidos
	_handsTickSFX = AssetManager::Instance()->GetSound(Sounds::CLICK);
	//---Texturas
	Texture2D btnTex = AssetManager::Instance()->GetTexture(Textures::BUTON1);

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

	//Botones por arriba
	const float bottomClockBigHandOffset = centerY + (hClock / 2.f) + 15.f; //con un poco de offset
	const float bigBtnWidth = 45.f;
	const float bigBtnHeight = 25.f;
	//---Botones manecilla grande
	Create<Button>("-", Rectangle{ centerX - bigBtnWidth, bottomClockBigHandOffset, bigBtnWidth, bigBtnHeight }, [this] {
		AudioManager::Instance()->PlaySound(_handsTickSFX);
		_bigHand->SetRotationOverload(-30.f);
		}, btnTex);
	Create<Button>("+", Rectangle{ centerX, bottomClockBigHandOffset, bigBtnWidth, bigBtnHeight }, [this] {
		AudioManager::Instance()->PlaySound(_handsTickSFX);
		_bigHand->SetRotationOverload(30.f);
		}, btnTex);

	//---Botones manecilla pequeña
	const float bottomClockSmallHandOffset = bottomClockBigHandOffset + 25.f;
	const float smallBtnWidth = 30.f;
	const float smallBtnHeight = 18.f;
	Create<Button>("-", Rectangle{ centerX - smallBtnWidth, bottomClockSmallHandOffset, smallBtnWidth, smallBtnHeight }, [this] {
		AudioManager::Instance()->PlaySound(_handsTickSFX);
		_smallHand->SetRotationOverload(-30.f);
		}, btnTex);
	Create<Button>("+", Rectangle{ centerX, bottomClockSmallHandOffset, smallBtnWidth, smallBtnHeight }, [this] {
		AudioManager::Instance()->PlaySound(_handsTickSFX);
		_smallHand->SetRotationOverload(30.f);
		}, btnTex);


}

void Bar::OnEnter() {

	auto* s = SceneManager::Instance();
	//---*Ejemplo de lo que podemos hacer con items o pjs respecto al reloj*---
	if (s->onDayState()) {
		_smallHand->SetActive(false);
	}else {}
	//------
}

void Bar::Update(float dt)
{
	Scene::Update(dt);

	//---*Ejemplo de lo que podemos hacer con items o pjs respecto al reloj*---
	const float r = _bigHand->getRotation();
	if (r == 90.f) { //3 de la tarde;
		if (!_smallHand->IsActive())
			_smallHand->SetActive(true);
	}
	//------
}

void Bar::OnExit() {

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