#pragma once

#include "Module.h"

class Component;
class ImporterMesh;
class ImporterMaterial;

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

	bool Import(std::string* file_to_import, std::string& output_file);
	Component* Load(std::string* file_to_load);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

private:
	ImporterMesh* MeshImporter = nullptr;
	ImporterMaterial* MaterialImporter = nullptr;
};