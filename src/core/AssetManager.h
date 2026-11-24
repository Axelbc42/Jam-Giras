#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"

namespace Textures {
	constexpr const char* BACKGROUND_PLAZA = "backgrounds/Plaza.png";
	constexpr const char* BACKGROUND_MENU = "backgrounds/FondoMenu.png";

	constexpr const char* ITEM_KEY_1 = "objects/key1.png";
	constexpr const char* ITEM_KEY_2 = "objects/key2.png";
	constexpr const char* CLOSED_DOOR = "objects/closedDoor.png";
	constexpr const char* OPENED_DOOR = "objects/openedDoor.png";

	constexpr const char* WHITE_ARROW = "ui/white_arrow.png";
	constexpr const char* CURSOR_DEFAULT = "ui/cursor.png";

	constexpr const char* BUTON1 = "UI/tile_0001.png";
}

namespace Sounds {
	constexpr const char* CLICK = "placeholder.wav";
	constexpr const char* OPEN_DOOR = "openDoor.wav";
}

namespace Musics {
	constexpr const char* MENU = "placeholder.mp3";
}

namespace Fonts {
	constexpr const char* MAIN = "MatchaStyle.ttf";
}

namespace FileName {
	constexpr const char* TUTORIAL = "Tutorial.txt";
	constexpr const char* TUTEJEMPLO = "Tutorial2.txt";
	constexpr const char* LOREMIPSUM = "Lorem.txt";

	//Replicar lo de arriba aqui
	const std::string COLECCION_FILES[] = { TUTORIAL ,TUTEJEMPLO ,LOREMIPSUM };
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