#include "ModuleSceneImporter.h"

#include "ComponentMesh.h"

ModuleSceneImporter::ModuleSceneImporter(Application* parent, bool start_enabled) : Module(parent, start_enabled)
{

}

ModuleSceneImporter::~ModuleSceneImporter()
{

}

bool ModuleSceneImporter::Init()
{
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
	return true;
}

void ModuleSceneImporter::DrawModuleImGui()
{

}

void ModuleSceneImporter::ImGuiModuleVariables()
{

}

bool ModuleSceneImporter::SaveBinary(const char* file_name, MeshData* mesh)
{
	return true;
}

bool ModuleSceneImporter::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleSceneImporter::LoadConf(JSON_Object* conf)
{
	return true;
}