#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui-1.51\imgui.h"

class GameObject;

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

	void PushFPSandMSPlot(uint fps, uint ms, uint mem);

	bool PlotsFreezed() const;

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

private:
	void ImGuiDrawMenuBar();
	void ImGuiConfigurationWindow();
	void ImGuiProfierWindow();
	void ImGuiModuleVariablesWindow();
	void ImGuiConsole();
	void ImGuiInspectorWindow();
	void ImGuiHierarchyWindow();
	void RecursiveDrawHierarchy(const GameObject* node);
	void ImGuiLoadFilePopUp();
	void RecursiveDrawDirectory(const char* directory);
	void ImGuiSaveFilePopUp();
	void ImGuiTimeManager();

private:
	bool startAsGame = false;
	GameObject* GameObjectSelected = nullptr;
	std::string Modes3D;
	bool showMenuBar = true;
	bool showTestWindow = false;
	bool showProfilerWindow = false;
	bool showModuleVariablesWindow = true;
	bool showConfigurationWindow = false;
	bool showConsoleWindow = true;
	bool showInspectorWindow = true;
	bool showHierarchyWindow = true;
	bool showLoadFilePopUp = false;
	bool showSaveFilePopUp = false;
	bool showTimeManager = true;
	bool freezeplots = false;
	bool active = false;
	bool ScrollDownConsole = true;
	std::list<std::string> console_logs;
	std::vector<float> fpsPlotData;
	std::vector<float> msPlotData;
	std::vector<float> memPlotData;
	ImGuiStyle style_custom;
	std::string LoadFileNameFileBrowser;
	std::string SaveFileNameFileBrowser;

private:
	char appnamestr[128];
};