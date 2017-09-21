#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void GetWindowSize(int& w, int& h)
	{
		w = w_width;
		h = w_height;
	}

	void SetFullscreen(bool set);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int w_width = 0;
	int w_height = 0;
	bool fullscreen = false;
	bool fullscreen_desktop = false;
};

#endif // __ModuleWindow_H__