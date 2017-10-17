#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(bool Active) : Component(Active, 1, ComponentType::Material_Component)
{

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
		char data[100] = "";
		uint data_size = sizeof data;

		sprintf_s(data, data_size, "Colors ID: %i", MaterialDataStruct.id_colors);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture ID: %i", MaterialDataStruct.id_texture);
		ImGui::Text(data);
		if (MaterialDataStruct.id_texture > 0)
		{
			static float texSize = 0.2f;
			ImGui::SliderFloat("Image Preview Size", &texSize, 0.0f, 1.0f, "%.2f");
			ImGui::Image((void*)MaterialDataStruct.id_texture, ImVec2(MaterialDataStruct.texture_w * texSize, MaterialDataStruct.texture_h * texSize), ImVec2(1, 1), ImVec2(0, 0));
		}
		sprintf_s(data, data_size, "Texture path: %s", MaterialDataStruct.texture_name.c_str());
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture W: %i", MaterialDataStruct.texture_w);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture H: %i", MaterialDataStruct.texture_h);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture D: %i", MaterialDataStruct.texture_d);
		ImGui::Text(data);
	}
}