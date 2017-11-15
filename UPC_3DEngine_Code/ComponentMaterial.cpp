#include "ComponentMaterial.h"
#include "ResourceTexture.h"

ComponentMaterial::ComponentMaterial(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Material_Component)
{
	if (Active) Enable();
}

ComponentMaterial::~ComponentMaterial()
{
	if (resourceTexture != nullptr)
		resourceTexture->UnLoad();
}

bool ComponentMaterial::Enable()
{
	return true;
}

bool ComponentMaterial::PreUpdate(float dt)
{
	return true;
}

bool ComponentMaterial::Update(float dt)
{
	return true;
}

bool ComponentMaterial::PostUpdate(float dt)
{
	return true;
}

bool ComponentMaterial::Disable()
{
	return true;
}

bool ComponentMaterial::CleanUp()
{
	return true;
}

void ComponentMaterial::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Material Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Material Component Active", &Active);

		if (resourceTexture == nullptr) return;

		char data[100] = "";
		uint data_size = sizeof data;

		sprintf_s(data, data_size, "Texture ID: %i", resourceTexture->TextureDataStruct.id_texture);
		ImGui::Text(data);
		if (resourceTexture->TextureDataStruct.id_texture > 0)
		{
			static float texSize = 0.2f;
			ImGui::SliderFloat("Image Preview Size", &texSize, 0.0f, 1.0f, "%.2f");
			ImGui::Image((void*)resourceTexture->TextureDataStruct.id_texture, ImVec2(resourceTexture->TextureDataStruct.texture_w * texSize, resourceTexture->TextureDataStruct.texture_h * texSize), ImVec2(1, 1), ImVec2(0, 0));
		}
		char title[1000] = "";
		sprintf_s(title, 1000, "Texture path: %s", resourceTexture->TextureDataStruct.texture_name.c_str());
		ImGui::Text(title);
		sprintf_s(data, data_size, "Texture W: %i", resourceTexture->TextureDataStruct.texture_w);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture H: %i", resourceTexture->TextureDataStruct.texture_h);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture D: %i", resourceTexture->TextureDataStruct.texture_d);
		ImGui::Text(data);
	}
}

void ComponentMaterial::SetResource(uint uuid)
{
	resourceTexture = (ResourceTexture*)App->resources->Get(uuid);
}

bool ComponentMaterial::SaveComponent(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "UUID", UUID);
	App->parsonjson->SetUInt(conf, "UUID_Parent", parent->GetUUID());
	App->parsonjson->SetBool(conf, "Active", Active);
	App->parsonjson->SetUInt(conf, "Type", type);
	if (resourceTexture != nullptr)
	{
		size_t bar_pos = resourceTexture->TextureDataStruct.texture_name.rfind("\\") + 1;
		std::string tex_name = resourceTexture->TextureDataStruct.texture_name.substr(bar_pos, resourceTexture->TextureDataStruct.texture_name.length());
		App->parsonjson->SetString(conf, "Texture_Path", tex_name.c_str());
	}
	else
		App->parsonjson->SetString(conf, "Texture_Path", "");
	return true;
}

bool ComponentMaterial::LoadComponent(JSON_Object* conf)
{
	UUID = App->parsonjson->GetUInt(conf, "UUID", 0);
	Active = App->parsonjson->GetBool(conf, "Active", true);
	const char* texture_name = App->parsonjson->GetString(conf, "Texture_Path", "");
	std::string tex_path = *App->importer->Get_Library_material_path() + "\\" + texture_name;
	//Load resource
	uint uuid = App->resources->LoadResource(tex_path.c_str());
	//Vinculate resource with component
	SetResource(uuid);
	//App->importer->LoadTexture(&tex_path, resourceTexture->TextureDataStruct);
	return true;
}