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
	//JSON_Value* va = json_value_init_array();
	//JSON_Array* array = json_value_get_array(va);
	//json_object_set_value(conf, /*array_name*/, va);
	//json_array_append_number(array, /*value*/);
	return true;
}

bool ComponentMaterial::LoadComponent(JSON_Object* conf)
{
	return true;
}