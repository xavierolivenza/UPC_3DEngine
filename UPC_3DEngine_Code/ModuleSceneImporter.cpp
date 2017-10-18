#include <experimental\filesystem>

#include "ModuleSceneImporter.h"

#include "ImporterMesh.h"
#include "ImporterMaterial.h"

ModuleSceneImporter::ModuleSceneImporter(Application* parent, bool start_enabled) : Module(parent, start_enabled)
{
	name = "scene_importer";
}

ModuleSceneImporter::~ModuleSceneImporter()
{

}

bool ModuleSceneImporter::Init()
{
	MeshImporter = new ImporterMesh();
	MeshImporter->Init();
	MaterialImporter = new ImporterMaterial();
	MaterialImporter->Init();
	
	//Create Assets folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Assets_path.c_str())) LOGP("Assets folder created");
	else LOGP("Assets folder already exists");

	//Create Library folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Library_path.c_str())) LOGP("Library folder created");
	else LOGP("Library folder already exists");

	//Create Library/Mesh folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Library_mesh_path.c_str())) LOGP("Mesh folder created");
	else LOGP("Mesh folder already exists");

	//Create Library/Material folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Library_material_path.c_str())) LOGP("Material folder created");
	else LOGP("Material folder already exists");

	//Iterate all Assets folder including files and directories
	for (auto& file_in_path : std::experimental::filesystem::recursive_directory_iterator(Assets_path.c_str()))
	{
		LOGP("%S", file_in_path.path().c_str());
		if (std::experimental::filesystem::is_regular_file(file_in_path.path()))
		{
			std::string output_file;
			if(Import(&file_in_path.path().string(), output_file)) LOGP("Regular File found and imported.");
			LOGP("Regular File found and import error.");
		}
	}

	return true;
}

bool ModuleSceneImporter::Start()
{
	return true;
}

update_status ModuleSceneImporter::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneImporter::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneImporter::CleanUp()
{
	MeshImporter->CleanUp();
	RELEASE(MeshImporter);
	MaterialImporter->CleanUp();
	RELEASE(MaterialImporter);
	return true;
}

void ModuleSceneImporter::DrawModuleImGui()
{

}

void ModuleSceneImporter::ImGuiModuleVariables()
{

}

bool ModuleSceneImporter::Import(std::string* file_to_import, std::string& output_file)
{
	bool ret = true;

	/*
	if (!std::experimental::filesystem::is_regular_file(file_to_import))
	{
		LOGP("The file you are trying to import is not a regular file: %s", file_to_import->c_str());
		return false;
	}
	*/




	return ret;
}

Component* ModuleSceneImporter::Load(std::string* file_to_load)
{
	return nullptr;
}

bool ModuleSceneImporter::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleSceneImporter::LoadConf(JSON_Object* conf)
{
	return true;
}