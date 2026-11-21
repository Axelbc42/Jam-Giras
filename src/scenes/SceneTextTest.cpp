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

    //Cargar todos los necesarios:
    DialogueManager::Instance()->loadDialogue(FileName::TUTORIAL);
}

void SceneTextTest::Update(float dt)
{

}

void SceneTextTest::Draw()
{
    //Podriamos llamarlo con los parametros que queramos editar
    DialogueManager::Instance()->showText(FileName::TUTORIAL);
}

void SceneTextTest::Release()
{
    // Liberar recursos propios de esta escena
}

void SceneTextTest::DrawDebugUI()
{
}