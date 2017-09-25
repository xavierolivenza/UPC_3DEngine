#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOGP("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGP("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		/*
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		w_width = width;
		w_height = height;
		*/
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, flags);

		if(window == NULL)
		{
			LOGP("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOGP("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
		SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::SaveConf(JSON_Object* conf) const
{
	json_object_set_number(conf, "width", w_width);
	json_object_set_number(conf, "height", w_height);
	json_object_set_string(conf, "title", title.c_str());
	json_object_set_boolean(conf, "fullscreen", fullscreen);
	json_object_set_boolean(conf, "resizable", resizable);
	json_object_set_boolean(conf, "borderless", borderless);
	json_object_set_boolean(conf, "fullscreen desktop", fullscreen_desktop);
	return true;
}

bool ModuleWindow::LoadConf(JSON_Object* conf)
{
	if (json_object_get_value(conf, "width") != NULL)
		w_width = json_object_get_number(conf, "width");
	if (json_object_get_value(conf, "height") != NULL)
		w_height = json_object_get_number(conf, "height");
	if (json_object_get_value(conf, "title") != NULL)
		title = json_object_get_string(conf, "title");
	if (json_object_get_value(conf, "fullscreen") != NULL)
		fullscreen = json_object_get_boolean(conf, "fullscreen");
	if (json_object_get_value(conf, "resizable") != NULL)
		fullscreen = json_object_get_boolean(conf, "resizable");
	if (json_object_get_value(conf, "borderless") != NULL)
		fullscreen = json_object_get_boolean(conf, "borderless");
	if (json_object_get_value(conf, "fullscreen desktop") != NULL)
		fullscreen = json_object_get_boolean(conf, "fullscreen desktop");
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	this->title = title;
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool set)
{
	if (set != fullscreen)
	{
		fullscreen = set;
		if (fullscreen)
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
			{
				LOGP("Could not fullscreen: %s\n", SDL_GetError());
				fullscreen_desktop = false;
			}
			else
				if (SDL_SetWindowFullscreen(window, 0) != 0)
					LOGP("Could not window: %s\n", SDL_GetError());
	}
}