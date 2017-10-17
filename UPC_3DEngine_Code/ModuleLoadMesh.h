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

	bool LoadGeometryFromModelFile(std::string* file);
	//bool LoadGeometryFromInternalBianryFile();

	//This will be deleted
	bool Load(std::string* file, std::vector<GeometryData>* meshDataOutput);
	//This will be deleted
	int LoadImageFromFile(const char* theFileName, uint& tex_w, uint& tex_h, uint& tex_d);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

	//This will be deleted
	const std::vector<GeometryData>* GetGeometryStructVector() const
	{
		return &geomData;
	}

private:
	void LoadGeometry(const aiScene* scene, GameObject* gameObject, const aiNode* MeshNode, const aiMesh* MeshInstance);
	//void LoadBuffers(Gameobject/components mesh+material);

	aiNode* SearchForMesh(aiNode* root, uint mesh_id);
	aiNode* SearchForMeshIterator(aiNode* root, uint mesh_id);
	//This will be deleted
	void LoadMeshGeometry(GeometryData& geomData, const aiScene* scene, const aiMesh* new_mesh, uint meshID, std::string& WorkingPath);
	//This will be deleted
	void LoadMeshBuffers(GeometryData& geomData);
	//This will be deleted
	bool CleanGeometryDataVector(std::vector<GeometryData>* meshDataVec);

private:
	std::string WorkingPath;
	//This will be deleted
	AABB SceneAABB;
	//This will be deleted
	std::string AssetsPath;
	//This will be deleted
	std::vector<GeometryData> geomData;
	//This will be deleted
	bool geomLoaded = false;
};