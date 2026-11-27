#include "AssetManager.h"

AssetManager* AssetManager::s_instance = nullptr;

AssetManager* AssetManager::Instance()
{
	if (s_instance == nullptr)
		s_instance = new AssetManager();
	return s_instance;
}

void AssetManager::Release()
{
	if (s_instance == nullptr)
		return;

	s_instance->UnloadAll();
	delete s_instance;
	s_instance = nullptr;
}

AssetManager::AssetManager()
{
	const char* candidatos[] = { "assets/", "../assets/", "../../assets/", "../../../assets/" };
	for (const char* c : candidatos)
	{
		if (DirectoryExists(c))
		{
			m_root = c;
			break;
		}
	}

	if (m_root.empty())
	{
		TraceLog(LOG_ERROR, "AssetManager: no se encuentra la carpeta assets (directorio de trabajo: %s)",
			GetWorkingDirectory());
		m_root = "assets/";
	}
}

// ---------------------------------------------------------------
// Carga (solo la primera vez; despues devuelve lo ya cargado)
// ---------------------------------------------------------------
Texture AssetManager::GetTexture(const std::string& name)
{
	auto it = m_textures.find(name);
	if (it == m_textures.end())
	{
		Texture tex = LoadTexture((m_root + "images/" + name).c_str());
		if (tex.id == 0)
			TraceLog(LOG_ERROR, "AssetManager: no se pudo cargar la textura %s", name.c_str());
		it = m_textures.insert({ name, tex }).first;
	}
	return it->second;
}

Sound AssetManager::GetSound(const std::string& name)
{
	auto it = m_sounds.find(name);
	if (it == m_sounds.end())
	{
		Sound snd = LoadSound((m_root + "audio/sfx/" + name).c_str());
		if (snd.frameCount == 0)
			TraceLog(LOG_ERROR, "AssetManager: no se pudo cargar el sonido %s", name.c_str());
		it = m_sounds.insert({ name, snd }).first;
	}
	return it->second;
}

Music AssetManager::GetMusic(const std::string& name)
{
	auto it = m_musics.find(name);
	if (it == m_musics.end())
	{
		Music mus = LoadMusicStream((m_root + "audio/music/" + name).c_str());
		if (mus.frameCount == 0)
			TraceLog(LOG_ERROR, "AssetManager: no se pudo cargar la musica %s", name.c_str());
		it = m_musics.insert({ name, mus }).first;
	}
	return it->second;
}

Font AssetManager::GetFont(const std::string& name)
{
	auto it = m_fonts.find(name);
	if (it == m_fonts.end())
	{
		// Rango de glifos: ASCII (32-126) + Latin-1 Supplement (160-255) -> tildes, ñ, ¿, ¡
		static int codepoints[95 + 96];
		static bool init = false;
		if (!init) {
			int n = 0;
			for (int c = 32; c <= 126; ++c)  codepoints[n++] = c;
			for (int c = 160; c <= 255; ++c) codepoints[n++] = c;
			init = true;
		}

		Font font = LoadFontEx((m_root + "fonts/" + name).c_str(), 64, codepoints, 95 + 96);
		if (font.texture.id == GetFontDefault().texture.id)
			TraceLog(LOG_ERROR, "AssetManager: no se pudo cargar la fuente %s", name.c_str());
		else {
			GenTextureMipmaps(&font.texture);
			SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);
		}
		it = m_fonts.insert({ name, font }).first;
	}
	return it->second;
}

std::string AssetManager::GetText(const std::string& name) //Solo abre el archivo y devuelve todo su contenido
{
	auto it = m_texts.find(name);
	if (it == m_texts.end())
	{
		char* fileText = LoadFileText((m_root + "data/" + name).c_str());
		if (!fileText) {
			TraceLog(LOG_ERROR, "AssetManager: No se pudo cargar %s", name.c_str());
			return "";
		}
		std::string content(fileText);
		UnloadFileText(fileText); // Liberamos el puntero de raylib si ya tenemos el std::string

		it = m_texts.insert({ name, content }).first;
	}
	return it->second;
}

// ---------------------------------------------------------------
// Descarga
// ---------------------------------------------------------------
void AssetManager::UnloadAll()
{
	for (auto& kv : m_textures) UnloadTexture(kv.second);
	for (auto& kv : m_sounds)   UnloadSound(kv.second);
	for (auto& kv : m_musics)   UnloadMusicStream(kv.second);
	for (auto& kv : m_fonts)    UnloadFont(kv.second);


	m_textures.clear();
	m_sounds.clear();
	m_musics.clear();
	m_fonts.clear();
	m_texts.clear();
}