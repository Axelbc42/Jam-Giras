#include "Creditos.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"
#include "../utilities/GameConstants.h"
#include "../entities/Button.h"

void Creditos::Init() {
	_clickSound = AssetManager::Instance()->GetSound(Sounds::CLICK);
	_background = AssetManager::Instance()->GetTexture(Textures::BACKGROUND_MENU);
	m_font = AssetManager::Instance()->GetFont(Fonts::MAIN);
	_backgroundHS = { 0, 0, GameConstants::kScreenWidth, GameConstants::kScreenHeight };
	//Cargar 4 textos?
	//Arte y narrativa
	//Programadores
	//Referencias de dominio publico
	//Assets no declarados como propios

	//3 botones, izquierda, derecha para los dialogos y volver para el menu.
}

void Creditos::Update(float dt) {

	Scene::Update(dt);
}

void Creditos::Draw() {

	Scene::Draw();
}