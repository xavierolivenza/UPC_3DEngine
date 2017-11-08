#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleParson.h"

ResourceTexture::ResourceTexture() : Resource(Resource::Type::texture)
{

}

ResourceTexture::~ResourceTexture()
{

}

void ResourceTexture::Save(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "uuid", uuid);
	App->parsonjson->SetString(conf, "file_assets", file.c_str());
	App->parsonjson->SetString(conf, "file_library", exported_file.c_str());
	App->parsonjson->SetString(conf, "file_time", file_date.c_str());
	App->parsonjson->SetUInt(conf, "type", type);
}

void ResourceTexture::Load(JSON_Object* conf)
{
	uuid = App->parsonjson->GetUInt(conf, "uuid", 0);
	file = App->parsonjson->GetString(conf, "file_assets", "");
	exported_file = App->parsonjson->GetString(conf, "file_library", "");
	type = (Resource::Type)App->parsonjson->GetUInt(conf, "type", Resource::Type::null);
	file_date = App->parsonjson->GetString(conf, "file_time", "");
}