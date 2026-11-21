#include "DialogueManager.h"

#include "raymath.h" 
#include <cassert>
#include "AssetManager.h"


DialogueManager* DialogueManager::_instance = nullptr;

void DialogueManager::Init() {
	//no se
}

void DialogueManager::unloadAll() {
	for (auto& kv : _dialogues)
		UnloadTextLines(kv.second._lines, kv.second._lineCount);
	_dialogues.clear();
}

void DialogueManager::Release() {
	if (_instance == nullptr) return;
	_instance->unloadAll();
	delete _instance;
	_instance = nullptr;
}

DialogueManager* DialogueManager::Instance() {
	if (_instance == nullptr) _instance = new DialogueManager;
	return _instance;
}

void DialogueManager::loadDialogue(const std::string fileName, int windowW,
	int windowH, int fontSize, Color color) {
	assert(AssetManager::Instance()); //tiene que existir antes, sino lo crea in situ?, no estoy seguro
	if (_dialogues.count(fileName)) return; //Si ya existe nada

	const int windowWidth = windowW;
	const int windowHeight = windowH;
	const int textTop = 25 + fontSize;

	const std::string content = AssetManager::Instance()->GetText(fileName);

	const char* contentStr = content.c_str();
	int lineCount = 0;
	char** lines = LoadTextLines(contentStr, &lineCount);

	int wrapWidth = windowWidth - 20;

	// 2. Tu bucle original de Word Wrapping
	for (int i = 0; i < lineCount; i++) {
		int j = 0;
		int lastSpace = 0;
		int lastWrapStart = 0;

		while (j <= strlen(lines[i])) {
			if (lines[i][j] == ' ' || lines[i][j] == '\0') {
				char before = lines[i][j];
				lines[i][j] = '\0';

				if (MeasureText(lines[i] + lastWrapStart, fontSize) > wrapWidth) {
					lines[i][lastSpace] = '\n';
					lastWrapStart = lastSpace + 1;
				}

				if (before != '\0') lines[i][j] = ' ';
				lastSpace = j;
			}
			j++;
		}
	}

	// Calc. altura scrollbar
	int textHeight = 0;
	for (int i = 0; i < lineCount; i++) {
		Vector2 size = MeasureTextEx(GetFontDefault(), lines[i], (float)fontSize, 2);
		textHeight += (int)size.y + 10;
	}

	Rectangle scrollBar = {
		.x = (float)windowWidth - 5,
		.y = 0,
		.width = 5,
		.height = (float)windowHeight * 100.0f / (float)(textHeight - windowWidth)
	};

	//Rellenar la info por si se necesita en otro lado?
	DialogueInfo diagInf;
	diagInf._fileName = fileName;
	diagInf._windowWidth = windowW;
	diagInf._windowHeight = windowH;
	diagInf._fontSize = fontSize;
	diagInf._textTop = textTop;
	diagInf._content = contentStr;
	diagInf._lineCount = lineCount;
	diagInf._lines = lines;
	diagInf._scrollBar = scrollBar;
	diagInf._textHeight = textHeight;
	diagInf._color = color;

	_dialogues[fileName] = diagInf;
}

void DialogueManager::showText(const std::string& assetName) {
	auto it = _dialogues.find(assetName);
	if (it == _dialogues.end()) {
		TraceLog(LOG_ERROR, "DialogueManager: No existe el archivo %s", assetName.c_str());
		return;
	}
	DialogueInfo& diagInf = it->second;

	// Draw
	//----------------------------------------------------------------------------------
	//BeginMode2D(cam);
	// Going through all the read lines
	for (int i = 0, t = diagInf._textTop; i < diagInf._lineCount; i++)
	{
		// Each time we go through and calculate the height of the text to move the cursor appropriately
		Vector2 size;
		if (strcmp(diagInf._lines[i], "")) {
			// Fix for empty line in the text file
			size = MeasureTextEx(GetFontDefault(), diagInf._lines[i], (float)diagInf._fontSize, 2);
		}
		else {
			size = MeasureTextEx(GetFontDefault(), " ", (float)diagInf._fontSize, 2);
		}

		DrawText(diagInf._lines[i], 10, t, diagInf._fontSize, diagInf._color);

		// Inserting extra space for real newlines,
		// wrapped lines are rendered closer together
		t += (int)size.y + 10;
	}
	//EndMode2D();

	// Header displaying which file is being read currently
	DrawRectangle(0, 0, diagInf._windowWidth, diagInf._textTop - 10, BEIGE);
	DrawText(TextFormat("File: %s", diagInf._fileName.c_str()), 10, 10, diagInf._fontSize, MAROON);

	DrawRectangleRec(diagInf._scrollBar, MAROON);
}

void DialogueManager::updateText() {
	//la logica del scrollBar aqui
}