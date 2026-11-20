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
		Font font = LoadFont((m_root + "fonts/" + name).c_str());
		// Si falla, raylib devuelve la fuente por defecto
		if (font.texture.id == GetFontDefault().texture.id)
			TraceLog(LOG_ERROR, "AssetManager: no se pudo cargar la fuente %s", name.c_str());
		it = m_fonts.insert({ name, font }).first;
	}
	return it->second;
}

Text AssetManager::GetText(const std::string& name)
{
	auto it = m_texts.find(name);
	if (it == m_texts.end())
	{
		Text textInfo;
		textInfo.text = LoadFileText((m_root + "data/" + name).c_str());
		// Si falla, raylib devuelve
		if (textInfo.text == nullptr)
			TraceLog(LOG_ERROR, "AssetManager: no se pudo cargar el archivo de texto %s", name.c_str());

		//Vamos a leer todo desde aqui

		int lineCount = 0;
		char** lines = LoadTextLines(textInfo.text, &lineCount);
		textInfo.lineCount = lineCount; //Por si luego necesitamos algo que calcular?
		textInfo.lines = lines;

		//estas 2 constantes son las que se fijaran por el cuadro de texto, no la pantalla
		//el cuadro tiene que ser obviamente <= dimensiones pantalla
		const int screenWidth = 300;
		const int screenHeight = 450;
		// Stylistic choises
		int fontSize = 20;
		int textTop = 25 + fontSize; // Top of the screen from where the text is rendered
		int wrapWidth = screenWidth - 20;

		// Wrap the lines as needed
		for (int i = 0; i < lineCount; i++)
		{
			int j = 0;
			int lastSpace = 0;          // Keeping track of last valid space to insert '\n'
			int lastWrapStart = 0;      // Keeping track of the start of this wrapped line.

			while (j <= strlen(lines[i]))
			{
				if (lines[i][j] == ' ' || lines[i][j] == '\0')
				{
					char before = lines[i][j];
					// Making a C Style string by adding a '\0' at the required location so that we can use the MeasureText function
					lines[i][j] = '\0';

					// Checking if the text has crossed the wrapWidth, then going back and inserting a newline
					if (MeasureText(lines[i] + lastWrapStart, fontSize) > wrapWidth)
					{
						lines[i][lastSpace] = '\n';

						// Since we added a newline the place of wrap changed so we update our lastWrapStart
						lastWrapStart = lastSpace + 1;
					}

					if (before != '\0') lines[i][j] = ' ';  // Resetting the space back
					lastSpace = j; // Since we encountered a new space we update our last encountered space location
				}

				j++;
			}

		}

		// Calculating the total height so that we can show a scrollbar
		int textHeight = 0;

		for (int i = 0; i < lineCount; i++)
		{
			Vector2 size = MeasureTextEx(GetFontDefault(), lines[i], (float)fontSize, 2);
			textHeight += (int)size.y + 10;
		}

		// A simple scrollbar on the side to show how far we have read into the file
		Rectangle scrollBar = {
			.x = (float)screenWidth - 5,
			.y = 0,
			.width = 5,
			.height = screenHeight * 100.0f / (textHeight - screenHeight) // Scrollbar height is just a percentage
		};


		it = m_texts.insert({ name, textInfo }).first;
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
	for (auto& kv : m_texts) { UnloadFileText(kv.second.text); UnloadTextLines(kv.second.lines, kv.second.lineCount);}
	


	m_textures.clear();
	m_sounds.clear();
	m_musics.clear();
	m_fonts.clear();
}