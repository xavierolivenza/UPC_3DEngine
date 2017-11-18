#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleParson.h"

ResourceMesh::ResourceMesh() : Resource(Resource::Type::mesh)
{

}

ResourceMesh::~ResourceMesh()
{

}

void ResourceMesh::CleanResource()
{
	SimpleMeshDataStruct.Clean();
}

void ResourceMesh::DrawResourceOptionsAndData()
{
	char title[50] = "";
	sprintf_s(title, 50, "UUID: %i", uuid);
	ImGui::Text(title);
	ImGui::Text(file.c_str());
	ImGui::Text(exported_file.c_str());
	ImGui::Text(file_date.c_str());
	sprintf_s(title, 50, "Type: %i", type);
	ImGui::Text(title);
	sprintf_s(title, 50, "Used: %i", loaded);
	ImGui::Text(title);

	static int ImportingPresetPick = 2;
	if (ImGui::Combo("Importing Preset", &ImportingPresetPick, "Fast\0Quality\0MaxQuality\0\0"))
	{
		switch (ImportingPresetPick)
		{
		case 0: SimpleMeshDataStruct.ImportingPreset = aiProcessPreset_TargetRealtime_Fast; break;
		case 1: SimpleMeshDataStruct.ImportingPreset = aiProcessPreset_TargetRealtime_Quality; break;
		case 2: SimpleMeshDataStruct.ImportingPreset = aiProcessPreset_TargetRealtime_MaxQuality; break;
		}
	}

	if (ImGui::Button("Import"))
	{
		//Reimport with new variables
	}
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