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
	int LoadImageFromFile(const char* theFileName);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

	//TEST

	const std::vector<GeometryData>* GetGeometryStructVector() const
	{
		return &geomData;
	}

public:
	uint Lenna_tex = 0;

private:
	std::string AssetsPath;
	std::vector<GeometryData> geomData;
	bool geomLoaded = false;
};