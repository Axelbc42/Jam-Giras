#include "SceneTextTest.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../core/DialogueManager.h"
#include "../utilities/GameConstants.h"

#include "imgui.h"

void SceneTextTest::Init()
{
	Scene::Init();

	m_hotspotTexture = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_PLAZA);
	m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);

	//DialogueManager::Instance()->OpenText(FileName::LOREMIPSUM); //Esto es lo que tiene que llamar al callback
	//DialogueManager::Instance()->OpenText(FileName::TUTEJEMPLO); //Esto es lo que tiene que llamar al callback
	//DialogueManager::Instance()->DontShowAny();
	//Ya lo dibujara Game
}

void SceneTextTest::Update(float dt)
{

}

void SceneTextTest::Draw()
{
}

void SceneTextTest::Release()
{
	// Liberar recursos propios de esta escena

}

void SceneTextTest::DrawDebugUI()
{

}