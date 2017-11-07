#include "Application.h"
#include "imgui-1.51\ImGuizmo.h"

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
	loadmesh = new ModuleLoadMesh(this);
	scene = new ModuleScene(this);
	importer = new ModuleSceneImporter(this);
	resources = new ModuleResources(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	AddModule(importer);
	AddModule(loadmesh);
	AddModule(resources);

	// Scenes
	AddModule(scene);
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
	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.crend(); ++item)
		RELEASE(*item);

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	parsonjson = new ParsonJSON("conf", false, false);
	parsonjson->Init();
	parsonjson->LoadModulesConfig();

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

	Time.ms_timer.Start();
	Time.last_sec_frame_time.Start();
	Time.startup_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	performance.frame_count++;
	Time.last_sec_frame_count++;
	Time.dt = (float)Time.ms_timer.Read() / 1000.0f;
	Time.ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (Time.last_sec_frame_time.Read() > 1000)
	{
		Time.last_sec_frame_time.Start();
		Time.prev_last_sec_frame_count = Time.last_sec_frame_count;
		Time.last_sec_frame_count = 0;
	}
	performance.frames_last_second = Time.prev_last_sec_frame_count;
	performance.average_framerate = (float)performance.frame_count / ((float)Time.startup_timer.Read() / 1000.0f);
	performance.miliseconds_per_frame = Time.ms_timer.Read();

	if (performance.capped_frames > 0)
	{
		uint cap = 1000 / performance.capped_frames;
		if (performance.miliseconds_per_frame < cap)
			SDL_Delay(cap - performance.miliseconds_per_frame);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	PrepareUpdate();
	float modified_DT = Time.dt * Time.TimeUpdate;

	if (Time.OneFrameForward) // if it's true, means TimeUpdate is 0.0f;
	{
		modified_DT = Time.dt;
		Time.OneFrameForward = false;
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item)
	{
		(*item)->ms_timer.Start();
		if ((*item) == camera) //Do not effect editor camera (if we do so, we can not move it)
			(*item)->PreUpdate(Time.dt);
		else
			(*item)->PreUpdate(modified_DT);
		PushMSToPreUpdate(*item, (*item)->ms_timer.Read());
	}

	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	ImGuizmo::BeginFrame();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item)
	{
		(*item)->ms_timer.Start();
		if ((*item) == camera) //Do not effect editor camera (if we do so, we can not move it)
			(*item)->Update(Time.dt);
		else
			(*item)->Update(modified_DT);
		PushMSToUpdate(*item, (*item)->ms_timer.Read());
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item)
	{
		(*item)->DrawModuleImGui();
	}
	ImGui::Render();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item)
	{
		(*item)->ms_timer.Start();
		if ((*item) == camera) //Do not effect editor camera (if we do so, we can not move it)
			(*item)->PostUpdate(Time.dt);
		else
			(*item)->PostUpdate(modified_DT);
		PushMSToPostUpdate(*item, (*item)->ms_timer.Read());
	}

	FinishUpdate();

	Time.GameSecSinceStartUp += modified_DT;

	if (Want_To_Close)
		ret = UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	parsonjson->SaveModulesConfig();
	RELEASE(parsonjson);
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.cend(); ++item)
	{
		(*item)->CleanUp();
		RELEASE(item._Ptr->_Myval);
	}
	list_modules.clear();
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

const PerformanceStruct* Application::GetPerformanceStruct() const
{
	return &performance;
}

const TimeManager* Application::GetTimeManagerStruct() const
{
	return &Time;
}

void Application::OpenLink(const char* link)
{
	ShellExecute(NULL, "open", link, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::PushMSToPreUpdate(Module* module, uint ms)
{
	if (App->engineUI->PlotsFreezed())
		return;

	static uint count = 0;

	if (count >= FPS_AND_MS_PLOT_DATA_LENGTH)
		for (int i = 0; i < FPS_AND_MS_PLOT_DATA_LENGTH - 1; i++)
		{
			module->ModulePreUpdateMs[i] = module->ModulePreUpdateMs[i + 1];
		}
	else
		count++;

	module->ModulePreUpdateMs[count - 1] = ms;
}

void Application::PushMSToUpdate(Module* module, uint ms)
{
	if (App->engineUI->PlotsFreezed())
		return;

	static uint count = 0;

	if (count >= FPS_AND_MS_PLOT_DATA_LENGTH)
		for (int i = 0; i < FPS_AND_MS_PLOT_DATA_LENGTH - 1; i++)
		{
			module->ModuleUpdateMs[i] = module->ModuleUpdateMs[i + 1];
		}
	else
		count++;

	module->ModuleUpdateMs[count - 1] = ms;
}

void Application::PushMSToPostUpdate(Module* module, uint ms)
{
	if (App->engineUI->PlotsFreezed())
		return;

	static uint count = 0;

	if (count >= FPS_AND_MS_PLOT_DATA_LENGTH)
		for (int i = 0; i < FPS_AND_MS_PLOT_DATA_LENGTH - 1; i++)
		{
			module->ModulePostUpdateMs[i] = module->ModulePostUpdateMs[i + 1];
		}
	else
		count++;

	module->ModulePostUpdateMs[count - 1] = ms;
}

void Application::WantToClose()
{
	Want_To_Close = true;
}

const std::list<Module*>* Application::GetModuleList() const
{
	return &list_modules;
}

uint& Application::GetFramerateCapModif()
{
	return performance.capped_frames;
}

EngineTimeStatus Application::GetEngineTimeStatus() const
{
	return Time.TimeStatus;
}

void Application::Play()
{
	Time.TimeStatus = EngineTimeStatus::play_unpause;
	UpdateEngineTimeStatusValue();
}

void Application::Stop()
{
	Time.TimeStatus = EngineTimeStatus::stop;
	UpdateEngineTimeStatusValue();
}

void Application::Pause()
{
	Time.TimeStatus = EngineTimeStatus::play_pause;
	UpdateEngineTimeStatusValue();
}

void Application::Frame()
{
	Time.OneFrameForward = true;
}

void Application::UpdateEngineTimeStatusValue(float value)
{
	if (value == -1.0f)
	{
		switch (Time.TimeStatus)
		{
		case EngineTimeStatus::play_unpause:
			Time.TimeUpdate = 1.0f;
			break;
		case EngineTimeStatus::play_pause:
		case EngineTimeStatus::stop:
			Time.TimeUpdate = 0.0f;
			break;
		}
	}
	/*
	else
	{
		if ((value >= 0.0f) && (value <= MAX_TIME_DISTORTION))
		{
			TimeUpdate = value;
			TimeStatus = EngineTimeStatus::play_custom;
		}
	}
	*/
}