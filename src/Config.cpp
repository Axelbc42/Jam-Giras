#include "Config.h"

//void Config::SetWindowMode(ConfigFlags flag)
//{
//	window_mode = flag;
//	SetWindowState(window_mode);
//}

void Config::ToggleWindowFullMode()
{
	is_maximized = !is_maximized;

	if (is_maximized) 
	{
		MaximizeWindow();
		SetBorderless(true);
	}
}

void Config::SetBorderless(bool has_border)
{
	ToggleBorderlessWindowed();
}
