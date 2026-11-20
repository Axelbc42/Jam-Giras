#include "raylib.h"

namespace Config 
{
	inline ConfigFlags window_mode = ConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_);
	// C++ 17, variable global de namespace

	inline float FPS = 60;

	inline float objective_frame_time = 1 / FPS;

	inline bool is_maximized = true;

	inline bool is_in_borderless = true;
}


namespace Config
{
	void ToggleWindowFullMode();
	//void SetWindowMode(ConfigFlags flag);
	// Cambia el window_mode a flag, y actualiza el comportamiento de la ventana al de flag

	void SetBorderless(bool has_border);

}