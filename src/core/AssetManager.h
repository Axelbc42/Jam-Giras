#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"

namespace Textures {
    constexpr const char* BACKGROUND_PLAZA = "backgrounds/Plaza.png";
    constexpr const char* CURSOR_DEFAULT = "ui/cursor.png";
    constexpr const char* ITEM_KEY = "objects/key.png";
    constexpr const char* BACKGROUND_MENU = "backgrounds/FondoMenu.png";
    constexpr const char* BUTON1 = "UI/tile_0001.png";
}

namespace Sounds {
    constexpr const char* CLICK = "placeholder.wav";
}

namespace Musics {
    constexpr const char* MENU = "placeholder.mp3";
}

namespace Fonts {
    constexpr const char* MAIN = "MatchaStyle.ttf";
}

namespace TextFile {
    constexpr const char* TUTORIAL = "Tutorial.txt";
    constexpr const char* TUTEJEMPLO = "Tutorial2.txt";
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
    Text    GetText(const std::string& name);

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
    std::unordered_map<std::string, Text>    m_texts;
};