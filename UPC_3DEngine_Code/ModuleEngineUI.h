#pragma once
#include "Module.h"
#include "Globals.h"

#define FPS_AND_MS_PLOT_DATA_LENGTH 100

//class ModuleMsData;

class ModuleEngineUI : public Module
{
public:
	ModuleEngineUI(Application* app, bool start_enabled = true);
	~ModuleEngineUI();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawModuleImGui();

	void PushNewConsoleLabel(std::string* newlabel);

	void GetEvent(SDL_Event* event);

	bool IsActive();

	void PushFPSandMSPlot(uint fps, uint ms);

private:
	bool showTestWindow = false;
	bool freezeplots = false;
	bool active = false;
	std::list<std::string> console_logs;
	std::vector<float> fpsPlotData;
	std::vector<float> msPlotData;

};
/*
class ModuleMsData
{
public:
	ModuleMsData(const char* modulename);
	~ModuleMsData();

	void PushMSToPreUpdate(uint ms);
	void PushMSToUpdate(uint ms);
	void PushMSToPostUpdate(uint ms);

private:
	enum PushType
	{
		PreUpdate,
		Update,
		PostUpdate
	};
	void CommonPush(uint ms, PushType type);

private:
	std::string ModuleName = "";
	std::vector<float> ModulePreUpdateMs;
	std::vector<float> ModuleUpdateMs;
	std::vector<float> ModulePostUpdateMs;
};
*/