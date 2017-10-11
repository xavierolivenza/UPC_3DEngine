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

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fullscreen_desktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, flags);

		/*
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		*/

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
	App->parsonjson->SetUInt(conf, "Max FPS Value", App->GetFramerateCapModif());
	App->parsonjson->SetInt(conf, "width", w_width);
	App->parsonjson->SetInt(conf, "height", w_height);
	App->parsonjson->SetString(conf, "title", title.c_str());
	App->parsonjson->SetBool(conf, "fullscreen", fullscreen);
	App->parsonjson->SetBool(conf, "resizable", resizable);
	App->parsonjson->SetBool(conf, "borderless", borderless);
	App->parsonjson->SetBool(conf, "fullscreen desktop", fullscreen_desktop);
	return true;
}

bool ModuleWindow::LoadConf(JSON_Object* conf)
{
	App->GetFramerateCapModif() = App->parsonjson->GetUInt(conf, "Max FPS Value", 0);
	w_width = App->parsonjson->GetInt(conf, "width", 1280);
	w_height = App->parsonjson->GetInt(conf, "height", 720);
	title = App->parsonjson->GetString(conf, "title", "Default Title");
	fullscreen = App->parsonjson->GetBool(conf, "fullscreen", false);
	resizable = App->parsonjson->GetBool(conf, "resizable", false);
	borderless = App->parsonjson->GetBool(conf, "borderless", false);
	fullscreen_desktop = App->parsonjson->GetBool(conf, "fullscreen desktop", false);
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

void ModuleWindow::ImGuiModuleVariables()
{
	char buffer[10];
	snprintf(buffer, sizeof buffer, "%if", w_width);
	if (ImGui::InputText("w_width", buffer, sizeof buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
		w_width = atoi(buffer);
	snprintf(buffer, sizeof buffer, "%if", w_height);
	if (ImGui::InputText("w_height", buffer, sizeof buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
		w_height = atoi(buffer);
	snprintf(buffer, sizeof buffer, "%if", App->GetFramerateCapModif());
	if (ImGui::InputText("MaxFPSValue", buffer, sizeof buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
		App->GetFramerateCapModif() = atoi(buffer);
}