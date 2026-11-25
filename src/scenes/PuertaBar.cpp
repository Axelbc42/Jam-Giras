#include "PuertaBar.h"
#include "../core/AssetManager.h"
#include "../core/SceneManager.h"

void PuertaBar::Init()
{
	_puertaBar.SetTexture(AssetManager::Instance()->GetTexture(Textures::CLOSED_DOOR));
	_puertaBar.SetRect(Rectangle{ 400.0f, 200.0f, 100.0f, 200.0f });
	_puertaBar.AddCallback([this]() {
		SceneManager::Instance()->ChangeScene("Bar");
		});
}

void PuertaBar::Update(float dt)
{
	_puertaBar.Update(dt);
}

void PuertaBar::Draw()
{
	_puertaBar.Draw();
}

void PuertaBar::Release()
{

}

void PuertaBar::DrawDebugUI()
{

}