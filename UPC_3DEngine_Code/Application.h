#pragma once

#include <cstdlib>

#include "Fluid_Studios_Memory_Manager\mmgr.h"
#include "Fluid_Studios_Memory_Manager\nommgr.h"

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleEngineUI.h"
#include "p2Log.h"

#include "Glew\include\glew.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	ModuleEngineUI* engineUI;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void WantToClose()
	{
		Want_To_Close = true;
	}

	void OpenLink(const char* link);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool Want_To_Close = false;
};

extern Application* App;