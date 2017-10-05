#pragma once

#include <list>
#include <vector>
#include <string>

#include "Globals.h"
#include "ModuleParson.h"
#include "Timer.h"

#include "MathGeoLib\MathGeoLib.h"
#include "MathGeoLib\MathBuildConfig.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#pragma comment (lib, "DevIL/libx86/DevIL.lib")

#define FPS_AND_MS_PLOT_DATA_LENGTH 100

class Application;
struct PhysBody3D;

class Module
{
public:

	Module(Application* parent, bool start_enabled = true) : enabled(start_enabled),
		ModulePreUpdateMs(FPS_AND_MS_PLOT_DATA_LENGTH), ModuleUpdateMs(FPS_AND_MS_PLOT_DATA_LENGTH), ModulePostUpdateMs(FPS_AND_MS_PLOT_DATA_LENGTH)
	{

	}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void DrawModuleImGui()
	{}

	virtual void ImGuiModuleVariables()
	{}

	virtual bool SaveConf(JSON_Object* conf) const
	{
		return true;
	}

	virtual bool LoadConf(JSON_Object* conf)
	{
		return true;
	}

private:
	bool enabled;
	
public:
	std::string name = "";
	std::vector<float> ModulePreUpdateMs;
	std::vector<float> ModuleUpdateMs;
	std::vector<float> ModulePostUpdateMs;
	Timer ms_timer;

};