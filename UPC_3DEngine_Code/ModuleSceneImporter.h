#pragma once

#include "Module.h"

struct MeshData;

class ModuleSceneImporter : public Module
{
public:

	ModuleSceneImporter(Application* parent, bool start_enabled = true);
	~ModuleSceneImporter();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawModuleImGui();
	void ImGuiModuleVariables();

	bool SaveBinary(const char* file_name, MeshData* mesh);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);
};