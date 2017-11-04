#include "ComponentMesh.h"
#include "ComponentTransform.h"

ComponentMesh::ComponentMesh(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Mesh_Component)
{
	if (Active) Enable();
}

ComponentMesh::~ComponentMesh()
{

}

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
	const Component* MeshComponent = parent->FindComponentFirst(ComponentType::Mesh_Component);
	if (MeshComponent != nullptr)
	{
		const Component* MaterialComponent = parent->FindComponentFirst(ComponentType::Material_Component);
		if (!MaterialComponent->IsActive())
			MaterialComponent = nullptr;
		App->renderer3D->DrawComponentMeshMaterial(parent->GetTransform(), (ComponentMesh*)MeshComponent, (ComponentMaterial*)MaterialComponent);
	}

	//Recalculate AABB of this mesh
	AABB LocalCopy;
	GetTransformedAABB(LocalCopy);

	if (DebugDrawAABB)
		App->renderer3D->DrawDebugBox(LocalCopy.CornerPoint(0), LocalCopy.CornerPoint(1), LocalCopy.CornerPoint(2), LocalCopy.CornerPoint(3), LocalCopy.CornerPoint(4), LocalCopy.CornerPoint(5), LocalCopy.CornerPoint(6), LocalCopy.CornerPoint(7), 1.0f, 1.0f, 0.0f);
	if (DebugDrawOBB)
		App->renderer3D->DrawDebugBox(MeshDataStruct.BoundOBox.CornerPoint(0), MeshDataStruct.BoundOBox.CornerPoint(1), MeshDataStruct.BoundOBox.CornerPoint(2), MeshDataStruct.BoundOBox.CornerPoint(3), MeshDataStruct.BoundOBox.CornerPoint(4), MeshDataStruct.BoundOBox.CornerPoint(5), MeshDataStruct.BoundOBox.CornerPoint(6), MeshDataStruct.BoundOBox.CornerPoint(7), 0.0f, 1.0f, 0.0f);

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
	MeshDataStruct.~MeshData();
	return true;
}

void ComponentMesh::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Mesh Component Active", &Active);

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
		sprintf_s(data, data_size, "Colors ID: %i", MeshDataStruct.id_colors);
		ImGui::Text(data);

		ImGui::InputFloat3("AABB Max Point", &MeshDataStruct.BoundBox.maxPoint[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("AABB Min Point", &MeshDataStruct.BoundBox.minPoint[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);

		ImGui::Checkbox("Debug Draw AABB", &DebugDrawAABB);
		ImGui::Checkbox("Debug Draw OBB", &DebugDrawOBB);
	}
}

bool ComponentMesh::SaveComponent(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "UUID", UUID);
	App->parsonjson->SetUInt(conf, "UUID_Parent", parent->GetUUID());
	App->parsonjson->SetBool(conf, "Active", Active);
	App->parsonjson->SetUInt(conf, "Type", type);
	App->parsonjson->SetString(conf, "Mesh_File_Name", MeshDataStruct.Mesh_File.c_str());
	return true;
}

bool ComponentMesh::LoadComponent(JSON_Object* conf)
{
	UUID = App->parsonjson->GetUInt(conf, "UUID", 0);
	Active = App->parsonjson->GetBool(conf, "Active", true);
	const char* MeshFile = App->parsonjson->GetString(conf, "Mesh_File_Name", "");
	std::string File_path = *App->importer->Get_Library_mesh_path() + "\\" + MeshFile;
	App->importer->LoadSimpleMesh(&File_path, MeshDataStruct);
	return true;
}

void ComponentMesh::GetTransformedAABB(AABB& TransformedBox) const
{
	TransformedBox = MeshDataStruct.BoundBox;
	ComponentTransform* transform = this->parent->GetTransform();
	const float4x4 matrix = *transform->GetMatrix();
	TransformedBox.TransformAsAABB(matrix.Transposed());
}