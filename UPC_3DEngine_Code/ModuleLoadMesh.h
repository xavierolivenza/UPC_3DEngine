#pragma once
#include "Module.h"
#include "Globals.h"

struct GeometryData;
struct MaterialData;
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

	int LoadImageFromFile(const char* theFileName, MaterialData* MaterailDataStruct);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

private:
	void LoadGeometry(const aiScene* scene, GameObject* gameObject, const aiNode* MeshNode, const aiMesh* MeshInstance);
	void LoadBuffers(ComponentMesh* meshComponent);

	aiNode* SearchForMesh(const aiNode* root, uint mesh_id) const;
	aiNode* SearchForMeshIterator(const aiNode* root, uint mesh_id) const;
	ComponentMaterial* SearchForTexture(const ComponentMaterial* materialComponent, const GameObject* root, std::string* texture_name) const;
	ComponentMaterial* SearchForTextureIterator(const ComponentMaterial* materialComponent, const GameObject* root, std::string* texture_name) const;

private:
	std::string WorkingPath;
};