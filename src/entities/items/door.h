#pragma once
#include "../GameObject.h"
#include "../../core/AssetManager.h"

class Door : public GameObject
{
public:

	void SetOpenTex(Texture t){
		openTex = t;
	}

	void OpenDoor() {
		if (openTex.id == 0) {
			openTex = AssetManager::Instance()->GetTexture(Textures::OPENED_DOOR);
		}

		SetTexture(openTex);
	}

private:
	Texture openTex{};
};