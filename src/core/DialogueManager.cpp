#include "DialogueManager.h"
#include "raymath.h" 
#include <cassert>
#include "AssetManager.h"
#include "imgui.h"

DialogueManager* DialogueManager::_instance = nullptr;

void DialogueManager::Init() {
	//Cargar TODO los necesarios
	const float centerX = GameConstants::kScreenWidth / 2.f;
	const float centerY = GameConstants::kScreenHeight / 2.f;

	//--FUENTES-- ?
	const Font mainFont = AssetManager::Instance()->GetFont(Fonts::MAIN);
	//--CAJAS-- h,w,x,y
	const float mboxWidth = GameConstants::kScreenWidth - 80.f;
	const float mboxHeight = 140.f;
	const Rectangle mainbox = createDialogueBox(mboxWidth, mboxHeight, 40.f, GameConstants::kScreenHeight - 180.f);

	const float smcboxWidth = 450.f;
	const float smcboxHeight = 150.f;
	const Rectangle smallCenterbox = createDialogueBox(smcboxWidth, smcboxHeight, centerX - smcboxWidth / 2.f, centerY - smcboxHeight / 2.f);

	const float cboxWidth = 450.f;
	const float cboxHeight = 300.f;
	const Rectangle nCenterbox = createDialogueBox(cboxWidth, cboxHeight, centerX - cboxWidth / 2.f, centerY - cboxHeight / 2.f);
	
	const float infoBoxWidth = GameConstants::kScreenWidth / 4;
	const float infoBoxHeight = 140.f;
	const Rectangle inventoryInfoBox = createDialogueBox(infoBoxWidth, infoBoxHeight, 20.f, GameConstants::kScreenHeight / 6);


	const float diaryBoxWidth = GameConstants::kScreenWidth / 3.1;
	const float diaryBoxHeight = GameConstants::kScreenHeight / 1.5f;
	const Rectangle diaryInfoBox = createDialogueBox(diaryBoxWidth, diaryBoxHeight, GameConstants::kScreenWidth / 1.9f, GameConstants::kScreenHeight / 5);
	
	//--DIALOGOS--
	//---Cargar todos los dialogos antes de usar---
	//---Ejemplos---
	DialogueManager::Instance()->LoadDialogue(FileName::TUTORIAL, mainbox, 14.f, BLUE, RED, BEIGE, mainFont);
	DialogueManager::Instance()->LoadDialogue(FileName::LOREMIPSUM, smallCenterbox, 18.f, GREEN, RED, BLACK, mainFont);
	DialogueManager::Instance()->LoadDialogue(FileName::TUTEJEMPLO, mainbox);

	//---Los del juego---
	DialogueManager::Instance()->LoadDialogue(FileName::DIALOGUE_BOX_1, nCenterbox, 14.f, WHITE, PURPLE, BLACK);
	DialogueManager::Instance()->LoadDialogue(FileName::DIALOGUE_BOX_2, smallCenterbox, 14.f, WHITE, PURPLE, BLACK);
	DialogueManager::Instance()->LoadDialogue(FileName::PLAZA, nCenterbox);

	//Dialogos
	DialogueManager::Instance()->LoadDialogue(FileName::NPC_1, mainbox, 20.f, WHITE, PINK, BLACK);

	//Pop-ups Inventario
	DialogueManager::Instance()->LoadDialogue(FileName::KEY_INFO, inventoryInfoBox, 18.f, WHITE, GOLD, BLACK);
	DialogueManager::Instance()->LoadDialogue(FileName::DIARY_INFO, inventoryInfoBox, 18.f, WHITE, DARKBROWN, BLACK);

	//Pop-ups Diario
	DialogueManager::Instance()->LoadDialogue(FileName::LUCES_DE_BOHEMIA_INFO, diaryInfoBox, 18.f, BLACK, BLANK, BLANK);
	DialogueManager::Instance()->LoadDialogue(FileName::CONTEXTO_INFO, diaryInfoBox, 18.f, BLACK, BLANK, BLANK);
	DialogueManager::Instance()->LoadDialogue(FileName::ESPERPENTO_INFO, diaryInfoBox, 18.f, BLACK, BLANK, BLANK);
	DialogueManager::Instance()->LoadDialogue(FileName::ESPEJOS_INFO, diaryInfoBox, 18.f, BLACK, BLANK, BLANK);
	DialogueManager::Instance()->LoadDialogue(FileName::CANOURO_INFO, diaryInfoBox, 18.f, BLACK, BLANK, BLANK);


	//--- Nombre de los archivos para el debug
	const int n = sizeof(FileName::COLECCION_FILES) / sizeof(FileName::COLECCION_FILES[0]);
	for (size_t i = 0; i < n; i++)
	{
		_filesAvaibles.push_back(FileName::COLECCION_FILES[i]);
	}

}

void DialogueManager::unloadAll() {
	_dialogues.clear();
}

Rectangle DialogueManager::createDialogueBox(float w, float h, float x, float y) {
	Rectangle r;
	r.width = w;r.height = h;
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

void DialogueManager::LoadDialogue(const std::string fileName, Rectangle box, float fontSize,
	Color textColor, Color headerColor, Color bodyColor, Font font) {
	assert(AssetManager::Instance()); //tiene que existir antes, sino lo crea in situ?, no estoy seguro
	if (_dialogues.count(fileName)) return; //Si ya existe nada

	//Todo el texto del archivo
	const std::string content = AssetManager::Instance()->GetText(fileName);

	DialogueInfo info;

	info._fileName = fileName;
	std::string titNoExt = fileName;
	info._title = titNoExt.erase(titNoExt.length() - 4); //para quitar el ".txt"
	info._content = content;
	BoxInfo b{ box ,headerColor, bodyColor };
	info._box = b;
	info._fontSize = fontSize;
	info._color = textColor;
	info._font = font;

	const float padding = 14.f; //horizontal y vertical

	//--- Calculo dinamico del titulo y su zona
	Vector2 titleSize = MeasureTextEx(font, info._title.c_str(), info._fontSize, info._lineSpacing);
	const float headerHeight = titleSize.y + padding * 2.f;

	//--- Region ocupable dentro de la caja
	info._viewport = {
	box.x + padding,
	box.y + headerHeight,
	box.width - padding * 2.f - 10.f,
	box.height - headerHeight - padding
	};

	info._scrollBar = {
	box.x - 5,
	info._viewport.y,
	5,
	10
	};

	const float wrapWidth = info._viewport.width; //se corta en el limite de horizontal
	std::string currentLine;
	std::string word;

	//--- Logica de lectura del contenido y calculo del wrapping
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

				Vector2 size = MeasureTextEx(font, candidate.c_str(), info._fontSize, info._lineSpacing);

				if (size.x > wrapWidth && !currentLine.empty())
				{
					info._lines.push_back(currentLine);
					currentLine = word;
				}
				else currentLine = candidate;

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

	//--- Calculo de lo que ocupa todo el texto verticalmente y el tope del scroll
	info._lineHeight = MeasureTextEx(font, "texto centinela pa k tenga ref", info._fontSize, info._lineSpacing).y + info._linePadding;
	info._textHeight = info._lineHeight * info._lines.size() + kTextOffsetY + kTextBottomPad;
	info._maxScroll = fmaxf(0.f, info._textHeight - info._viewport.height);

	//--- Scrollbar
	float thumbHeight = info._viewport.height;

	if (info._textHeight > info._viewport.height)
	{
		thumbHeight = info._viewport.height * (info._viewport.height / info._textHeight);
	}

	thumbHeight = fmaxf(20.f, thumbHeight); // Evitar que sea demasiado pequeño

	info._scrollBar = {
		box.x + box.width - 8.f,
		info._viewport.y,
		4.f,
		thumbHeight
	};

	_dialogues[fileName] = info; //Se guarda la info
}

void DialogueManager::Close() {
	if (_active.empty()) return;
	_lastActive = _active;
	const std::string next = _dialogues.at(_active)._next;
	_active.clear(); //quitamos la key

	if (!next.empty()) OpenText(next);
}

void DialogueManager::CloseAll() {
	if (_active.empty()) return;
	//Para que no muestre nada
	_dialogues.at(_active)._next.clear();
	_active.clear();
}

void DialogueManager::Draw() {
	if (_active.empty()) return;

	DialogueInfo& diagInf = _dialogues.at(_active); //Si esto falla chao pescao
	Rectangle box = diagInf._box._rect;

	//-- Mostrar Fondo y bordes
	DrawRectangleRec(box, diagInf._box._bodyColor);
	if (diagInf._box._bodyColor.a != 0)
		DrawRectangleLinesEx(box, 2.f, diagInf._color);

	//-- Mostrar header
	DrawRectangle(box.x, box.y, box.width, diagInf._viewport.y - box.y, diagInf._box._headerColor);
	DrawTextEx(diagInf._font, diagInf._title.c_str(), Vector2(box.x + 10.f, box.y + 12.f), diagInf._fontSize,
		diagInf._lineSpacing, diagInf._color);

	//-- Mostrar el texto en el vw
	BeginScissorMode(diagInf._viewport.x, diagInf._viewport.y, diagInf._viewport.width, diagInf._viewport.height);

	float y = diagInf._viewport.y - diagInf._scrollY;

	for (const std::string& line : diagInf._lines)
	{
		DrawTextEx(diagInf._font, line.c_str(), Vector2(diagInf._viewport.x, y + kTextOffsetY), diagInf._fontSize, diagInf._lineSpacing, diagInf._color);

		y += diagInf._lineHeight;
	}

	EndScissorMode();

	//-- Mostrar el scrollbar
	if (diagInf._maxScroll <= 0.f) return; //Solo mostrar si hay que desplazar
	// Fondo de la scrollbar
	DrawRectangleRec({ diagInf._scrollBar.x, diagInf._viewport.y, 4.f, diagInf._viewport.height }, DARKGRAY);
	// Indicador
	DrawRectangleRec(diagInf._scrollBar, GREEN);
}

void DialogueManager::OpenText(const std::string& assetName) {
	if (_active == assetName) return; //Para evitar sobreescribir en draws o updates
	auto it = _dialogues.find(assetName);
	if (it == _dialogues.end()) {
		TraceLog(LOG_ERROR, "DialogueManager: No existe el archivo %s", assetName.c_str());
		return;
	}
	_active = it->first;
	it->second._scrollY = 0.f; //Siempre arriba
}

void DialogueManager::SetNext(const std::string& from, const std::string& to) {
	_dialogues.at(from)._next = to;
}

void DialogueManager::Update(float dt) {
	if (_active.empty()) return;

	DialogueInfo& info = _dialogues.at(_active);
	if (CheckCollisionPointRec(GetMousePosition(), info._viewport)) //solo funciona con el mouse dentro de la cajita
	{
		float wheel = GetMouseWheelMove();
		info._scrollY -= wheel * 20.f;
		info._scrollY = Clamp(info._scrollY, 0.f, info._maxScroll);
	}

	//--- Posibilidad de cerrar el texto al clickar en pantalla (teniendo en cuenta la vent de imgui)
	if (info._maxScroll == info._scrollY && !ImGui::GetIO().WantCaptureMouse) //estamos en el final del viewport
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Close();
		}

	//% del scrollbar que se muestra
	float scrollRatio = 0.f;
	if (info._maxScroll > 0.f)
		scrollRatio = info._scrollY / info._maxScroll;
	float availableTravel = info._viewport.height - info._scrollBar.height;
	info._scrollBar.y = info._viewport.y + availableTravel * scrollRatio;


	// --- PROVISIONALLLLLLL feedback para saber que puede quitar el texto al clickar
	_elapsed += dt;
	if (_elapsed >= 1000.f) _elapsed -= 1000.f;
	float osc = (std::sin(_elapsed * _VEL_OSC) + 1) * 0.5f; //[0,1]
	float alpha = Lerp(0.95f, 0.90f, osc);

	// Solo modulamos el alfa si el fondo no es transparente (los del diario usan BLANK)
	if (info._box._bodyColor.a != 0) {
		if (info._maxScroll == info._scrollY) {
			info._box._bodyColor.a = (unsigned char)(alpha * 255.f);
		}
		else {
			info._box._bodyColor.a = (unsigned char)(255.f * 0.90f);
		}
	}
}

void DialogueManager::DrawDebugUI() {
	if (ImGui::Button("Cerrar/Abrir Opciones dialogo")) {
		if (isActive()) Close();
		else if (!_lastActive.empty()) OpenText(_lastActive);
	}

	//siempre visibles
	for (size_t i = 0; i < _filesAvaibles.size(); i++)
	{
		const std::string f = _filesAvaibles[i];
		char label[64];
		snprintf(label, sizeof(label), "%s", f.c_str());

		ImGui::PushID(int(i));
		if (ImGui::CollapsingHeader(label))
			if (ImGui::Button("Abrir ")) {
				DialogueManager::Instance()->Close();
				DialogueManager::Instance()->OpenText(f);
			}
		ImGui::PopID();
	}

	if (_active.empty()) return;
	//editar la cajita
	DialogueInfo& info = _dialogues.at(_active);
	ImGui::Text("Cajita Texto");

	ImGui::DragFloat2("Posicion", &info._box._rect.x, 1.f);
	ImGui::DragFloat2("Tamano", &info._box._rect.width, 1.f);


}