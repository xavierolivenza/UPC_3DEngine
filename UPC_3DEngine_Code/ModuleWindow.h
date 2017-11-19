#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
	friend class ModuleEngineUI;
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void SetTitle(const char* title);

	void SetWindowSize(int w, int h)
	{
		w_width = w;
		w_height = h;
	}

	void GetWindowSize(int& w, int& h) const
	{
		w = w_width;
		h = w_height;
	}

	const std::string* GetTitle() const
	{
		return &title;
	}

	void SetFullscreen(bool set);

	void ImGuiModuleVariables();

	float GetAspectRatio()
	{
		return (float)w_width / (float)w_height;
	}

	bool GetLibraryHidden()
	{
		return Library_Hidden;
	}

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int w_width = 0;
	int w_height = 0;
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool fullscreen_desktop = false;
	bool Library_Hidden = false;
	std::string title;
};

#endif // __ModuleWindow_H__