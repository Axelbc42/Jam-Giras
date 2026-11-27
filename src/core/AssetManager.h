#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"

namespace Textures {
	constexpr const char* BACKGROUND_PLAZA = "backgrounds/Plaza.png";
	constexpr const char* BACKGROUND_MENU = "backgrounds/MenuBG.png";

	constexpr const char* ITEM_KEY_1 = "objects/key1.png";
	constexpr const char* ITEM_KEY_2 = "objects/key2.png";
	constexpr const char* CLOSED_DOOR = "objects/closedDoor.png";
	constexpr const char* OPENED_DOOR = "objects/openedDoor.png";
	constexpr const char* CLOCK = "objects/clock.png";
	constexpr const char* HAND = "objects/hand.png";

	constexpr const char* MIRROR_1 = "sprites/mirror1.png";
	constexpr const char* MIRROR_2 = "sprites/mirror2.png";
	constexpr const char* MIRROR_3 = "sprites/mirror3.png";
	constexpr const char* MIRROR_4 = "sprites/mirror4.png";
	constexpr const char* MIRROR_5 = "sprites/mirror5.png";
	constexpr const char* MIRROR_6 = "sprites/mirror6.png";
	constexpr const char* MIRRORS[] = { MIRROR_1, MIRROR_2, MIRROR_3, MIRROR_4, MIRROR_5, MIRROR_6 };

	constexpr const char* DIARY_FRAMES = "UI/diary.png";
	constexpr const char* CLOSED_DIARY = "UI/libritoCerrado.png";

	constexpr const char* WHITE_ARROW = "UI/white_arrow.png";
	constexpr const char* CURSOR_DEFAULT = "UI/cursor.png";

	constexpr const char* BUTON1 = "UI/tile_0001.png";
}

namespace Sounds {
	constexpr const char* CLICK = "click1.ogg";
	constexpr const char* OPEN_DOOR = "openDoor.wav";
}

namespace Musics {
	constexpr const char* MENU = "placeholder.mp3";
}

namespace Fonts {
	//constexpr const char* MAIN = "MatchaStyle.ttf";
	//constexpr const char* MAIN = "Handwriting-Regular.otf";
	constexpr const char* MAIN = "Abbasy Calligraphy Typeface.ttf"; //Este de momento el mejor
}

namespace FileName {
	constexpr const char* TUTORIAL = "Tutorial.txt";
	constexpr const char* TUTEJEMPLO = "Tutorial2.txt";
	constexpr const char* LOREMIPSUM = "Lorem.txt";

	//Ya para uso en el videojuego
	constexpr const char* DIALOGUE_BOX_1 = "Cuadros de Texto 1.txt";
	constexpr const char* DIALOGUE_BOX_2 = "Cuadros de Texto 2.txt";
	constexpr const char* PLAZA = "La Plaza.txt";

	//Diálogos
	constexpr const char* NPC_1 = "NPC_1.txt";

	//Descrpciones inventario
	constexpr const char* KEY_INFO = "KEY_ITEM_INFO.txt";
	constexpr const char* DIARY_INFO = "DIARY_ITEM_INFO.txt";

	//Descripcioens nodos diario
	constexpr const char* LUCES_DE_BOHEMIA_INFO = "LUCES_DE_BOHEMIA_INFO.txt";
	constexpr const char* CONTEXTO_INFO = "CONTEXTO_INFO.txt";
	constexpr const char* ESPERPENTO_INFO = "ESPERPENTO_INFO.txt";
	constexpr const char* ESPEJOS_INFO = "ESPEJOS_INFO.txt";
	constexpr const char* CANOURO_INFO = "CANOURO_INFO.txt";


	//Replicar lo de arriba aqui
	const std::string COLECCION_FILES[] = { TUTORIAL ,TUTEJEMPLO ,LOREMIPSUM,DIALOGUE_BOX_1,DIALOGUE_BOX_2,PLAZA };
}


class AssetManager
{
public:
	static AssetManager* Instance();
	static void Release();

	Texture GetTexture(const std::string& name);
	Sound   GetSound(const std::string& name);
	Music   GetMusic(const std::string& name);
	Font    GetFont(const std::string& name);
	std::string   GetText(const std::string& name); //todo el contenido

	//GetModel() y GetShader() también se pueden implementar si los necesitamos para modelos 3D y shaders

	void UnloadAll();

	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

private:
	AssetManager();
	~AssetManager() = default;

	static AssetManager* s_instance;

	std::string m_root;      // "assets/", "../assets/"... segun donde se encuentre

	std::unordered_map<std::string, Texture> m_textures;
	std::unordered_map<std::string, Sound>   m_sounds;
	std::unordered_map<std::string, Music>   m_musics;
	std::unordered_map<std::string, Font>    m_fonts;
	std::unordered_map<std::string, std::string>   m_texts;
};