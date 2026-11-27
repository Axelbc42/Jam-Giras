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
	void LoadDialogue(std::string fileName, Rectangle box, float fontSize = 12,
		Color textColor = RED, Color headerColor = BEIGE, Color bodyColor = BLACK, Font = GetFontDefault());
	void OpenText(const std::string& fileName);
	void SetNext(const std::string& from, const std::string& to);
	bool isActive() const { return !_active.empty(); }
	void Close();
	void CloseAll(); //PROVISIONAL
	void Draw(); //La key
	void Update(float dt);
	void DrawDebugUI();

	DialogueManager() = default;
	~DialogueManager() = default;
	DialogueManager(DialogueManager&) = delete;
	DialogueManager& operator=(const DialogueManager&) = delete;

private: //privados para que no accedais por error sin el wrapper
	void unloadAll();
	Rectangle createDialogueBox(float w, float h, float x, float y);
private:

	static DialogueManager* _instance;

	struct BoxInfo {
		Rectangle _rect{};
		Color _headerColor = BEIGE;
		Color _bodyColor = BLACK;
	};

	struct DialogueInfo {
		std::string _fileName;
		std::string _title = ""; //podria interpretarse como titulo del objeto que se inspecciona
		std::string _content = "";
		std::string _next = "";  // vacio = no hay siguiente
		std::vector<std::string> _lines; //pa sustituir char** _lines = nullptr; ya no hace falta

		float _fontSize = 12.f;
		float _lineSpacing = 2.f; //Solo esta definido aqui
		float _linePadding = 5.f; //Solo esta definido aqui
		float _lineHeight = 10.f; //se calcula luego bien en el load


		float _textHeight = 0.f;
		float _scrollY = 0.f;
		float _maxScroll = 0.f;

		BoxInfo _box{}; //ya con esto sabemos lo que queremos que mida la cajita
		Rectangle _viewport{};
		Rectangle _scrollBar{};


		Font _font = GetFontDefault();
		Color _color = WHITE;
	};

	static constexpr float kTextOffsetY = 8.f;
	static constexpr float kTextBottomPad = 6.f;

	std::string _active = ""; //asumo que solo un texto por pantalla
	std::string _lastActive;
	float _elapsed = 0.f;
	static constexpr float _VEL_OSC = 4.f;
	std::unordered_map<std::string, DialogueInfo> _dialogues;
	std::vector< std::string> _filesAvaibles;
};

