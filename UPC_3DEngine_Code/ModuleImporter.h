#pragma once

#include "Module.h"

class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* parent, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawModuleImGui();
	void ImGuiModuleVariables();

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);
};