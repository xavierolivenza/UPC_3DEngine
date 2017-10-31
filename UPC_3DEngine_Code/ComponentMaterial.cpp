#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Material_Component)
{
	if (Active) Enable();
}

ComponentMaterial::~ComponentMaterial()
{}

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

		char data[100] = "";
		uint data_size = sizeof data;

		sprintf_s(data, data_size, "Texture ID: %i", MaterialDataStruct.id_texture);
		ImGui::Text(data);
		if (MaterialDataStruct.id_texture > 0)
		{
			static float texSize = 0.2f;
			ImGui::SliderFloat("Image Preview Size", &texSize, 0.0f, 1.0f, "%.2f");
			ImGui::Image((void*)MaterialDataStruct.id_texture, ImVec2(MaterialDataStruct.texture_w * texSize, MaterialDataStruct.texture_h * texSize), ImVec2(1, 1), ImVec2(0, 0));
		}
		char title[1000] = "";
		sprintf_s(title, 1000, "Texture path: %s", MaterialDataStruct.texture_name.c_str());
		ImGui::Text(title);
		sprintf_s(data, data_size, "Texture W: %i", MaterialDataStruct.texture_w);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture H: %i", MaterialDataStruct.texture_h);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture D: %i", MaterialDataStruct.texture_d);
		ImGui::Text(data);
	}
}

bool ComponentMaterial::SaveComponent(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "UUID", UUID);
	App->parsonjson->SetUInt(conf, "UUID_Parent", parent->GetUUID());
	App->parsonjson->SetBool(conf, "Active", Active);
	App->parsonjson->SetUInt(conf, "Type", type);
	size_t bar_pos = MaterialDataStruct.texture_name.rfind("\\") + 1;
	std::string tex_name = MaterialDataStruct.texture_name.substr(bar_pos, MaterialDataStruct.texture_name.length());
	App->parsonjson->SetString(conf, "Texture_Path", tex_name.c_str());
	return true;
}

bool ComponentMaterial::LoadComponent(JSON_Object* conf)
{
	return true;
}