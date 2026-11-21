#pragma once
#include <iostream>
#include "../utilities/GameConstants.h"
#include <unordered_map>
#include "raylib.h"
class DialogueManager
{
public:
	static DialogueManager* Instance();
	static void Release();

	void Init();

	//el tamaño del display lo decidimos aqui
	void loadDialogue(std::string fileName, int windowW = GameConstants::kScreenWidth / 6,
		int windowH = GameConstants::kScreenHeight / 6, int fontSize = 12, Color color= RED); //Ver si se puede elegir FONT
	void showText(const std::string& fileName); //La key
	void updateText();
private: //privados para que no accedais por error sin el wrapper
	DialogueManager() = default;
	~DialogueManager() = default;
	DialogueManager(DialogueManager&) = delete;
	DialogueManager& operator=(const DialogueManager&) = delete;

	void unloadAll();
private:

	static DialogueManager* _instance;

	struct DialogueInfo {
		std::string _fileName;
		int _windowWidth = -1;
		int _windowHeight = -1;
		int _textTop = -1;
		int _fontSize = -1;
		std::string _content = "";
		int _lineCount = 0;
		char** _lines = nullptr;
		Rectangle _scrollBar{};
		int _textHeight = 0;
		Color _color{};
	};

	std::unordered_map<std::string, DialogueInfo> _dialogues;
};

