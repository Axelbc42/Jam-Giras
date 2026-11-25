#pragma once
#include "../Clickable.h"
#include "../../core/AssetManager.h"
#include "../../core/AudioManager.h"
#include "../../core/DialogueManager.h"

class DialogueInteractuable : public Clickable
{
public:
	DialogueInteractuable(const std::string& assetName, bool hover, bool clickable){
		
		if(clickable)
			AddCallback([this, assetName]() {
				DialogueManager::Instance()->OpenText(assetName);
			});

		if(hover)
			SetOnHover([this, assetName]() {
			DialogueManager::Instance()->OpenText(assetName);
				});
	}

private:

};
