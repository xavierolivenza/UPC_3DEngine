#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui-1.51\imgui.h"

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

private:
	void ImGuiDrawMenuBar();
	void ImGuiConfigurationWindow();
	void ImGuiProfierWindow();
	void ImGuiModuleVariablesWindow();
	void ImGuiConsole();
	void ImGuiPropertiesWindow();

private:
	std::string Modes3D;
	bool showTestWindow = false;
	bool showProfilerWindow = true;
	bool showModuleVariablesWindow = true;
	bool showConfigurationWindow = true;
	bool showConsoleWindow = true;
	bool showPropertiesWindow = true;
	bool freezeplots = false;
	bool active = false;
	bool ScrollDownConsole = true;
	std::list<std::string> console_logs;
	std::vector<float> fpsPlotData;
	std::vector<float> msPlotData;
	std::vector<float> memPlotData;
	ImGuiStyle style_custom;

private:
	char appnamestr[128];
};