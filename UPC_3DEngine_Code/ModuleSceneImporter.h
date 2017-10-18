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
	std::string Assets_path = "..\\Game\\Assets";
	std::string Library_path = "..\\Game\\Library";
	std::string Library_mesh_path = "..\\Game\\Library\\Mesh";
	std::string Library_material_path = "..\\Game\\Library\\Material";

	ImporterMesh* MeshImporter = nullptr;
	ImporterMaterial* MaterialImporter = nullptr;
};