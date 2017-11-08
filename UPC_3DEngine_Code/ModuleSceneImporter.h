#pragma once

#include "Module.h"

class Component;
struct MeshData;
struct MaterialData;
class ImporterMesh;
class ImporterMaterial;
struct aiNode;
struct aiScene;

class ModuleSceneImporter : public Module
{
private:
	struct AssimpNodeTransOwnFile
	{
		float3 pos = { 0.0f,0.0f,0.0f };
		float3 scale = { 1.0f,1.0f,1.0f };
		Quat rot = { 0.0f,0.0f,0.0f,1.0f };
		uint mesh = 0;
	};
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

	bool ImportFBX(std::string* file_to_import, std::string& output_file);
	bool Load(std::string* file_to_load);
	bool LoadSimpleMesh(std::string* file_to_load, MeshData& Data);
	bool LoadTexture(std::string* file_to_load, MaterialData& DataMaterial);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	const std::string* Get_Assets_path() const;
	const std::string* Get_Library_path() const;
	const std::string* Get_Library_mesh_path() const;
	const std::string* Get_Library_material_path() const;
	const std::string* Get_Settings_path() const;
	const std::string* Get_Scenes_path() const;
	const std::string* Get_Mesh_Extention() const;
	const std::string* Get_FBXComponents_Extention() const;

private:
	aiNode* SearchForMesh(const aiNode* root, uint mesh_id) const;
	aiNode* SearchForMeshIterator(const aiNode* root, uint mesh_id) const;
	bool ImportFBXComponents(const aiScene* scene, const std::string* file_to_import, const std::vector<std::string>* FBXComponents, std::string& output_file);
	bool LoadFBXComponents(const std::string* file_to_load);
	void RecursiveIterateFBX(const aiNode* node, std::list<const aiNode*>& list);

public:
	ImporterMesh* MeshImporter = nullptr;
	ImporterMaterial* MaterialImporter = nullptr;

private:
	std::string WorkingPath; //Used to load textures from same path as fbx
	std::string Assets_path = "..\\Game\\Assets";
	std::string Library_path = "..\\Game\\Library";
	std::string Library_mesh_path = "..\\Game\\Library\\Mesh";
	std::string Library_material_path = "..\\Game\\Library\\Material";
	std::string Settings_path = "..\\Game\\Settings"; //Imgui save file cannot load this because when its loaded App is nullptr, so remember to change ther the path too f you change this (imgui.cpp line:752)
	std::string Scenes_path = "..\\Game\\Assets\\Scenes";
	std::string Mesh_Extention = "MeshAlvOli";
	std::string FBXComponents_Extention = "GameObjectMeshAlvOli";

	Timer CheckAssetsImport;
	uint miliseconds_check = 3000;

};