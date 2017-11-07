#include "Application.h"
#include "ModuleSceneImporter.h"
#include "ImporterMesh.h"
#include "ImporterMaterial.h"

#include "ModuleResources.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{

}

bool ModuleResources::Init()
{
	return true;
}

bool ModuleResources::Start()
{
	return true;
}

update_status ModuleResources::PreUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleResources::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleResources::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

void ModuleResources::DrawModuleImGui()
{

}

void ModuleResources::ImGuiModuleVariables()
{

}

bool ModuleResources::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleResources::LoadConf(JSON_Object* conf)
{
	return true;
}

uint ModuleResources::Find(const char* file_in_assets) const
{
	return 0;
}

uint ModuleResources::ImportFile(const char* new_file_in_assets, Resource::Type type)
{
	uint ret = 0;
	bool imported = false;
	std::string output;

	switch (type)
	{
	case Resource::mesh: imported = App->importer->ImportFBX(&std::string(new_file_in_assets), output); break;
	case Resource::texture: imported = App->importer->MaterialImporter->Save(&std::string(new_file_in_assets), output); break;
	}

	if (imported)
	{
		Resource* res = CreateNewResource(type);
		res->file = new_file_in_assets;
		res->exported_file = output;
		ret = res->GetUID();

		ParsonJSON* parsonjson = new ParsonJSON(output.c_str(), true, true, false);
		bool MetaSaved = parsonjson->Init();
		if (MetaSaved)
			MetaSaved = parsonjson->SaveResource(res);
		RELEASE(parsonjson);
		if (MetaSaved) LOGP("SaveScene Success, file: %s", output.c_str());
		else LOGP("SaveScene Failure, file: %s", output.c_str());
	}

	return ret;
}

const Resource* ModuleResources::Get(uint uid) const
{
	return nullptr;
}

Resource* ModuleResources::Get(uint uid)
{
	std::map<uint, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

Resource* ModuleResources::CreateNewResource(Resource::Type type)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case Resource::texture: ret = (Resource*) new ResourceTexture(); break;
	case Resource::mesh: ret = (Resource*) new ResourceMesh(); break;
	}
	if (ret != nullptr)
		resources.insert(std::pair<uint, Resource*>(ret->GetUID(), ret));
	return ret;
}