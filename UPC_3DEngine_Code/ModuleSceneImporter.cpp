#include "ModuleSceneImporter.h"

#include "Importer.h"
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
	MaterialImporter = new ImporterMaterial();
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
	RELEASE(MeshImporter);
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
	return true;
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