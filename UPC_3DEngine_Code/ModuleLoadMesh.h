#pragma once
#include "Module.h"
#include "Globals.h"

struct GeometryData;

class ModuleLoadMesh : public Module
{
public:
	ModuleLoadMesh(Application* app, bool start_enabled = true);
	~ModuleLoadMesh();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Load(std::string* file, std::vector<GeometryData>& meshDataOutput);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

	//TEST
private:
	std::vector<GeometryData> geomData;
	bool geomLoaded = false;
};