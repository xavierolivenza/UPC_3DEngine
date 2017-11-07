#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleParson.h"

ResourceMesh::ResourceMesh() : Resource(Resource::Type::mesh)
{

}

ResourceMesh::~ResourceMesh()
{

}

void ResourceMesh::Save(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "uuid", uuid);
	App->parsonjson->SetString(conf, "file_assets", file.c_str());
	App->parsonjson->SetString(conf, "file_library", exported_file.c_str());
	App->parsonjson->SetString(conf, "file_time", file_date.c_str());
	App->parsonjson->SetUInt(conf, "type", type);
}

void ResourceMesh::Load(JSON_Object* conf)
{
	uuid = App->parsonjson->GetUInt(conf, "uuid", 0);
	file = App->parsonjson->GetString(conf, "file_assets", "");
	exported_file = App->parsonjson->GetString(conf, "file_library", "");
	type = (Resource::Type)App->parsonjson->GetUInt(conf, "type", Resource::Type::null);
	file_date = App->parsonjson->GetString(conf, "file_time", "");
}