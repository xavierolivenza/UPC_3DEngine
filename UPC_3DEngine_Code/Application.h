#pragma once

#include <cstdlib>

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
#include "ModuleEngineUI.h"
#include "ModuleParson.h"
#include "ModuleScene.h"
#include "ModuleSceneImporter.h"
#include "ModuleResources.h"
#include "p2Log.h"

#include "Timer.h"

#include "Glew\include\glew.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"

struct PerformanceStruct
{
	int frame_count = 0;
	uint average_framerate = 0;
	uint frames_last_second = 0;
	uint miliseconds_per_frame = 0;
	uint capped_frames = 0;
};

enum EngineTimeStatus
{
	play_unpause,
	play_pause,
	//play_pause_frame,
	stop
	//play_custom //TimeUpdate != 0.0 or 1.0f and > 0.0f (no negative time), Time distortion
};

struct TimeManager
{
	//Realtime
	Timer	ms_timer;
	Timer	startup_timer;
	Timer	last_sec_frame_time;
	float	dt = 0.0f;
	uint prev_last_sec_frame_count = 0;
	uint last_sec_frame_count = 0;
	//GameTime
	float GameSecSinceStartUp = 0.0f;
	EngineTimeStatus TimeStatus = EngineTimeStatus::stop;
	float TimeUpdate = 0.0f; //0.0f: pause, 1.0f: normal time, other: time distortion
	float TimeDistortion = 1.0f;
	bool OneFrameForward = false;
};

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	const std::list<Module*>* GetModuleList() const;
	uint& GetFramerateCapModif();
	const PerformanceStruct* GetPerformanceStruct() const;
	TimeManager& GetTimeManagerStruct();

	void OpenLink(const char* link);
	void WantToClose();

	//TimeManager
	EngineTimeStatus GetEngineTimeStatus() const;
	void Play();
	void Stop();
	void Pause();
	void Frame();
	void UpdateEngineTimeStatusValue(float value = -1.0f); //Send value > 0.0f to affect this and change to a custom EngineTimeStatus

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void PushMSToPreUpdate(Module* module, uint ms);
	void PushMSToUpdate(Module* module, uint ms);
	void PushMSToPostUpdate(Module* module, uint ms);

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleEngineUI* engineUI;
	ModuleScene* scene;
	ModuleSceneImporter* importer;
	ModuleResources* resources;

	ParsonJSON* parsonjson;

private:
	std::list<Module*> list_modules;
	PerformanceStruct performance;

	//Time Manager
	TimeManager Time;
	
	bool Want_To_Close = false;
};

extern Application* App;