#include "ModuleImporter.h"

ModuleImporter::ModuleImporter(Application* parent, bool start_enabled) : Module(parent, start_enabled)
{

}

ModuleImporter::~ModuleImporter()
{

}

bool ModuleImporter::Init()
{
	return true;
}

bool ModuleImporter::Start()
{
	return true;
}

update_status ModuleImporter::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	return true;
}

void ModuleImporter::DrawModuleImGui()
{

}

void ModuleImporter::ImGuiModuleVariables()
{

}

bool ModuleImporter::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleImporter::LoadConf(JSON_Object* conf)
{
	return true;
}