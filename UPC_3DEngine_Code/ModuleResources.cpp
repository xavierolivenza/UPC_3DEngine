#include <experimental/filesystem>

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
	for (std::map<uint, Resource*>::iterator item = resources.begin(); item != resources.end(); ++item)
		RELEASE(item->second);
	resources.clear();
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

uint ModuleResources::ImportFile(const char* new_file_in_assets)
{
	uint ret = 0;
	bool imported = false;
	std::string output;

	std::string extention = new_file_in_assets;
	size_t bar_pos = extention.rfind(".") + 1;
	extention = extention.substr(bar_pos, extention.length());
	Resource::Type type = Resource::Type::null;
	if((extention == "fbx") || (extention == "FBX") || (extention == "obj") || (extention == "dae"))
		type = Resource::Type::mesh;
	if ((extention == "png") || (extention == "jpg") || (extention == "tga") || (extention == "dds"))
		type = Resource::Type::texture;

	/*
	//Better but generates runtime memory leaks
	//(when you leave the execution, all memeory are deallocated, but in runtime acumulate, we don't underestand why)
	Resource::Type type = Resource::Type::null;
	if (App->importer->MeshImporter->AssimpCanLoad(new_file_in_assets))
		type = Resource::Type::mesh;
	else if (App->importer->MaterialImporter->DevilCanLoad(new_file_in_assets))
		type = Resource::Type::texture;
	*/

	/**/
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

		for (std::experimental::filesystem::recursive_directory_iterator::value_type file_in_path : std::experimental::filesystem::recursive_directory_iterator(App->importer->Get_Assets_path()->c_str()))
		{
			if (std::experimental::filesystem::is_regular_file(file_in_path.path()))
			{
				//LOGP("%S", file_in_path.path().string().c_str());
				if (res->file == file_in_path.path().string().c_str())
				{
					std::experimental::filesystem::file_time_type ftime = std::experimental::filesystem::last_write_time(file_in_path.path());
					std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
					res->file_date = std::asctime(std::localtime(&cftime));
				}
			}
		}

		size_t bar_pos = res->file.rfind("\\") + 1;
		std::string filepath_name = res->file.substr(0, bar_pos);
		filepath_name += res->file.substr(bar_pos, res->file.length());
		filepath_name += ".meta";
		ParsonJSON* parsonjson = new ParsonJSON(filepath_name.c_str(), true, true, false);
		bool MetaSaved = parsonjson->Init();
		if (MetaSaved)
			MetaSaved = parsonjson->SaveResource(res);
		RELEASE(parsonjson);
		if (MetaSaved) LOGP("SaveScene Success, file: %s", filepath_name.c_str());
		else LOGP("SaveScene Failure, file: %s", filepath_name.c_str());
	}
	/**/
	return ret;
}

bool ModuleResources::ReimportResource(Resource& res)
{
	bool ret = false;
	switch (res.type)
	{
	case Resource::mesh: ret = App->importer->ImportFBX(&res.file, res.exported_file); break;
	case Resource::texture: ret = App->importer->MaterialImporter->Save(&res.file, res.exported_file); break;
	}
	return ret;
}

const Resource* ModuleResources::Get(uint uid) const
{
	std::map<uint, Resource*>::const_iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
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

Resource* ModuleResources::GetResource(const char* path)
{
	for (std::map<uint, Resource*>::iterator item = resources.begin(); item != resources.end(); ++item)
	{
		if (item._Ptr->_Myval.second->file == path)
			return item._Ptr->_Myval.second;
	}
	return nullptr;
}

const std::map<uint, Resource*>* ModuleResources::GetResourcesMap() const
{
	return &resources;
}