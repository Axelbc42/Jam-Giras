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
	void LoadDialogue(std::string fileName, Rectangle box, float fontSize = 12, Color color = RED, Font = GetFontDefault());
	void OpenText(const std::string& fileName);
	void Close();
	void Draw(); //La key
	void Update();
	void DrawDebugUI();
private: //privados para que no accedais por error sin el wrapper
	DialogueManager() = default;
	~DialogueManager() = default;
	DialogueManager(DialogueManager&) = delete;
	DialogueManager& operator=(const DialogueManager&) = delete;

	void unloadAll();
	const Rectangle& createDialogueBox(float h, float w, float x, float y);
private:

	static DialogueManager* _instance;

	struct DialogueInfo {
		std::string _fileName; 
		std::string _title = ""; //podria interpretarse como titulo del objeto que se inspecciona
		std::string _content = "";
		std::vector<std::string> _lines; //pa sustituir char** _lines = nullptr; ya no hace falta

		float _fontSize = 12.f;
		float _lineSpacing = 2.f;
		float _linePadding = 5.f;
		float _lineHeight = 10.f; //se calcula luego bien en el load


		float _textHeight = 0.f;
		float _scrollY = 0.f;
		float _maxScroll = 0.f;

		Rectangle _box{}; //ya con esto sabemos lo que queremos que mida la cajita
		Rectangle _viewport{};
		Rectangle _scrollBar{};


		Font _font = GetFontDefault();
		Color _color = WHITE;
	};

	std::string _active = ""; //asumo que solo un texto por pantalla
	std::unordered_map<std::string, DialogueInfo> _dialogues;
};

