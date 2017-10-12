#pragma once
#include "Module.h"
#include "Globals.h"

struct GeometryData;
struct aiScene;
struct aiNode;
struct aiMesh;

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

	bool Load(std::string* file, std::vector<GeometryData>* meshDataOutput);
	int LoadImageFromFile(const char* theFileName, uint& tex_w, uint& tex_h);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

//TEST
	const std::vector<GeometryData>* GetGeometryStructVector() const
	{
		return &geomData;
	}

private:
	void LoadMeshGeometry(GeometryData& geomData, const aiScene* scene, const aiMesh* new_mesh, uint meshID, std::string& WorkingPath);
	void LoadMeshBuffers(GeometryData& geomData);


	aiNode* SearchForMesh(aiNode* root, uint mesh_id);
	aiNode* SearchForMeshIterator(aiNode* root, uint mesh_id);
	bool CleanGeometryDataVector(std::vector<GeometryData>* meshDataVec);

private:
	AABB SceneAABB;
	std::string AssetsPath;
	std::vector<GeometryData> geomData;
	bool geomLoaded = false;
};