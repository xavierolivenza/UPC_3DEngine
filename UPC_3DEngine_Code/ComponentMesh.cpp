#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(bool Active) : Component(Active, 1, ComponentType::Mesh_Component)
{

}

ComponentMesh::~ComponentMesh()
{}

bool ComponentMesh::Enable()
{
	return true;
}

bool ComponentMesh::PreUpdate(float dt)
{
	return true;
}

bool ComponentMesh::Update(float dt)
{
	return true;
}

bool ComponentMesh::PostUpdate(float dt)
{
	return true;
}

bool ComponentMesh::Disable()
{
	return true;
}

bool ComponentMesh::CleanUp()
{
	return true;
}

void ComponentMesh::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char data[100] = "";
		uint data_size = sizeof data;

		sprintf_s(data, data_size, "Number of faces: %i", MeshDataStruct.num_faces);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Vertices number: %i", MeshDataStruct.num_vertices);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Vertices ID: %i", MeshDataStruct.id_vertices);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Indices number: %i", MeshDataStruct.num_indices);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Indices ID: %i", MeshDataStruct.id_indices);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Normals ID: %i", MeshDataStruct.id_normals);
		ImGui::Text(data);
		sprintf_s(data, data_size, "Texture Coords ID: %i", MeshDataStruct.id_texture_coords);
		ImGui::Text(data);

		ImGui::InputFloat3("AABB Max Point", &MeshDataStruct.BoundBox.maxPoint[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("AABB Min Point", &MeshDataStruct.BoundBox.minPoint[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
	}
}