#include "Application.h"

#include <gl/GL.h>
#include <gl/GLU.h>

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	player = new ModulePlayer(this);
	engineUI = new ModuleEngineUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(player);
	
	//camera need to be below player
	AddModule(camera);

	// Renderer last!
	AddModule(renderer3D);

	AddModule(engineUI);
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.crend(); ++item) {
		delete((*item));
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend() && ret == true; ++item) {
		ret = (item._Ptr->_Myval)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOGP("Application Start --------------");

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend() && ret == true; ++item) {
		ret = (item._Ptr->_Myval)->Start();
	}

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item) {
		(*item)->PreUpdate(dt);
	}


	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item) {
		(*item)->Update(dt);
	}

	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item) {
		(*item)->DrawModuleImGui();
	}
	ImGui::Render();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item) {
		(*item)->PostUpdate(dt);
	}

	FinishUpdate();

	if (Want_To_Close)
		ret = UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item)
	{
		(*item)->CleanUp();
		delete *item;
	}
	list_modules.clear();
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenLink(const char* link)
{
	ShellExecute(NULL, "open", link, NULL, NULL, SW_SHOWMAXIMIZED);
}