#include "DialogueManager.h"

#include "raymath.h" 
#include <cassert>
#include "AssetManager.h"
#include "imgui.h"


DialogueManager* DialogueManager::_instance = nullptr;

void DialogueManager::Init() {
	//Cargar TODO los necesarios
	//--FUENTES-- ?
	const Font mainFont = AssetManager::Instance()->GetFont(Fonts::MAIN);
	//--CAJAS--
	const Rectangle mainbox = createDialogueBox(140.f, GameConstants::kScreenWidth - 80.f, 40.f, GameConstants::kScreenHeight - 180.f);

	//--DIALOGOS--
	//---Cargar todos los dialogos antes de usar---
	DialogueManager::Instance()->LoadDialogue(FileName::TUTORIAL, mainbox);
	DialogueManager::Instance()->LoadDialogue(FileName::LOREMIPSUM, mainbox, 24.f, GREEN, mainFont);
	DialogueManager::Instance()->LoadDialogue(FileName::TUTEJEMPLO, mainbox);

}

void DialogueManager::unloadAll() {
	_dialogues.clear();
}

const Rectangle& DialogueManager::createDialogueBox(float h, float w, float x, float y) {
	Rectangle r;
	r.height = h;r.width = w;
	r.x = x;r.y = y;
	return r;
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

void DialogueManager::LoadDialogue(const std::string fileName, Rectangle box, float fontSize, Color color, Font font) {
	assert(AssetManager::Instance()); //tiene que existir antes, sino lo crea in situ?, no estoy seguro
	if (_dialogues.count(fileName)) return; //Si ya existe nada

	//Todo el texto del archivo
	const std::string content = AssetManager::Instance()->GetText(fileName);

	DialogueInfo info;

	info._fileName = fileName;
	std::string titNoExt = fileName;
	info._title = titNoExt.erase(titNoExt.length() - 4); //para quitar el ".txt"
	info._content = content;
	info._box = box;
	info._fontSize = fontSize;
	info._color = color;
	info._font = font;


	const float padding = 12.f; //horizontal y vertical
	const float headerHeight = 25.f;

	//la region visible
	info._viewport = {
	box.x + padding,
	box.y + headerHeight,
	box.width - padding * 2.f - 10.f,
	box.height - headerHeight - padding
	};

	info._scrollBar = {
	.x = (float)box.x - 5,
	.y = info._viewport.y,
	.width = 5,
	.height = 10
	};

	const float wrapWidth = info._viewport.width; //se corta en el limite de arriba

	std::string currentLine;
	std::string word;

	for (int i = 0; i <= (int)content.size(); i++)
	{
		char c;
		if (i < content.size()) {
			c = content[i];
		}
		else  c = '\n';


		if (c == ' ' || c == '\n')
		{
			if (!word.empty())
			{
				//logica para ver si cabe la palabra en la linea actual
				//parecida a la logica del ejemplo de raylib pero no supe continuarlo. Shotout to claudia aqui 
				std::string candidate = currentLine.empty()
					? word
					: currentLine + " " + word;

				Vector2 size = MeasureTextEx(
					font,
					candidate.c_str(),
					info._fontSize,
					info._lineSpacing
				);

				if (size.x > wrapWidth && !currentLine.empty())
				{
					info._lines.push_back(currentLine);
					currentLine = word;
				}
				else
				{
					currentLine = candidate;
				}

				word.clear();
			}
			if (c == '\n')
			{
				info._lines.push_back(currentLine);
				currentLine.clear();
			}
		}
		else //sigue leyendo la palabra
		{
			word += c;
		}
	}

	if (!currentLine.empty()) //para la linea sobrante (por si no termino en un salto de linea)
		info._lines.push_back(currentLine);

	// Altura total del contenido
	info._lineHeight = MeasureTextEx(font, "texto centinela pa k tenga ref", info._fontSize, info._lineSpacing).y + info._linePadding;

	info._textHeight = info._lineHeight * info._lines.size();

	info._maxScroll = fmaxf(0.f, info._textHeight - info._viewport.height);

	_dialogues[fileName] = info;
}

void DialogueManager::Close() {
	_active.clear(); //quitamos la key
}

void DialogueManager::Draw() {
	if (_active.empty()) return;

	DialogueInfo& diagInf = _dialogues.at(_active); //Si esto falla chao pescao

	Rectangle box = diagInf._box;

	// Fondo, editar luego por si se quiere otro color de fondo o bordes
	DrawRectangleRec(box, Fade(BLACK, 0.85f));
	DrawRectangleLinesEx(box, 2.f, diagInf._color);

	//El header, altura y color habria que definir constante o que sea parametrizable
	DrawRectangle(box.x, box.y, box.width, 25, BEIGE);
	DrawTextEx(diagInf._font, diagInf._title.c_str(), Vector2(box.x + 10.f, box.y + 8.f), diagInf._fontSize,
		diagInf._lineSpacing, diagInf._color);

	BeginScissorMode(diagInf._viewport.x, diagInf._viewport.y, diagInf._viewport.width, diagInf._viewport.height);

	float y = diagInf._viewport.y - diagInf._scrollY;

	for (const std::string& line : diagInf._lines)
	{
		DrawTextEx(diagInf._font, line.c_str(), Vector2(diagInf._viewport.x, y + 8.f), diagInf._fontSize, diagInf._lineSpacing, diagInf._color);

		y += diagInf._lineHeight;
	}

	EndScissorMode();
}

void DialogueManager::OpenText(const std::string& assetName) {
	auto it = _dialogues.find(assetName);
	if (it == _dialogues.end()) {
		TraceLog(LOG_ERROR, "DialogueManager: No existe el archivo %s", assetName.c_str());
		return;
	}
	_active = it->first;
}

void DialogueManager::Update() {
	if (_active.empty()) return;
	//Si se llega al maximo de abajo del scrollBar enseñar algo para cerrar el dialogo
	//Para que se cierre el dialogo solo con clickar fuera creo que habria que acoplar logica en el GAME

	DialogueInfo& info = _dialogues.at(_active);
	if (CheckCollisionPointRec(GetMousePosition(), info._viewport)) //solo funciona con el mouse en la cajita
	{
		float wheel = GetMouseWheelMove();

		info._scrollY -= wheel * 20.f;

		info._scrollY = Clamp(info._scrollY, 0.f, info._maxScroll);

		info._scrollBar.y = Lerp((float)info._viewport.y, (float)info._viewport.y + info._viewport.height - info._scrollBar.height, 0.5);
	}


}

void DialogueManager::DrawDebugUI() {
	if (_active.empty()) return;

	DialogueInfo& info = _dialogues.at(_active);

	ImGui::Text("Cajita Texto");

	ImGui::DragFloat2("Posicion", &info._box.x, 1.f);
	ImGui::DragFloat2("Tamano", &info._box.width, 1.f);
}