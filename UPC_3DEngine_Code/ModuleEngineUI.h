#pragma once
#include "Module.h"
#include "Globals.h"

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

	void PushNewConsoleLabel(std::string* newlabel);

	bool IsActive()
	{
		return active;
	}

private:
	bool active = false;
	std::list<std::string> console_logs;

};
