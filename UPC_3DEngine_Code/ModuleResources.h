#pragma once
#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include <map>

class ModuleResources : public Module
{
public:

	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

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

	uint Find(const char* file_in_assets) const;
	uint ImportFile(const char* new_file_in_assets);
	bool ReimportResource(Resource& res);
	const Resource* Get(uint uid) const;
	Resource* Get(uint uid);
	Resource* CreateNewResource(Resource::Type type);
	Resource* GetResource(const char* path);

	const std::map<uint, Resource*>* GetResourcesMap() const;

private:
	uint uuid = 0;
	std::map<uint, Resource*> resources;
};