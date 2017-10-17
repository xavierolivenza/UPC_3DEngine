#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(bool Active) : Component(Active, 1, ComponentType::Mesh_Component)
{

}

ComponentMesh::~ComponentMesh()
{
	if (DebugDrawAABB_vertices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawAABB_id_vertices);
		RELEASE_ARRAY(DebugDrawAABB_vertices);
	}
	if (DebugDrawAABB_indices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawAABB_id_indices);
		RELEASE_ARRAY(DebugDrawAABB_indices);
	}
	if (DebugDrawOBB_vertices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawOBB_id_vertices);
		RELEASE_ARRAY(DebugDrawOBB_vertices);
	}
	if (DebugDrawOBB_indices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawOBB_id_indices);
		RELEASE_ARRAY(DebugDrawOBB_indices);
	}
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
	if (DebugDrawAABB && (DebugDrawAABB_id_vertices != 0))
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glLineWidth(2.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, DebugDrawAABB_id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawAABB_id_indices);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, NULL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(1.0f);
	}
	if (DebugDrawOBB && (DebugDrawOBB_id_vertices != 0))
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glLineWidth(2.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, DebugDrawOBB_id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawOBB_id_indices);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, NULL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(1.0f);
	}
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
		sprintf_s(data, data_size, "Colors ID: %i", MeshDataStruct.id_colors);
		ImGui::Text(data);

		ImGui::InputFloat3("AABB Max Point", &MeshDataStruct.BoundBox.maxPoint[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("AABB Min Point", &MeshDataStruct.BoundBox.minPoint[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);

		ImGui::Checkbox("Debug Draw AABB", &DebugDrawAABB);
		ImGui::Checkbox("Debug Draw OBB", &DebugDrawOBB);
	}
}

void ComponentMesh::GenerateAABBDraw()
{
	//Clean if there is another aabb allocated
	if (DebugDrawAABB_vertices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawAABB_id_vertices);
		RELEASE_ARRAY(DebugDrawAABB_vertices);
	}
	if (DebugDrawAABB_indices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawAABB_id_indices);
		RELEASE_ARRAY(DebugDrawAABB_indices);
	}

	//Box math
	DebugDrawAABB_vertices = new float[24];
	float vertices[] =
	{ 
		MeshDataStruct.BoundBox.CornerPoint(0).x,MeshDataStruct.BoundBox.CornerPoint(0).y,MeshDataStruct.BoundBox.CornerPoint(0).z,
		MeshDataStruct.BoundBox.CornerPoint(1).x,MeshDataStruct.BoundBox.CornerPoint(1).y,MeshDataStruct.BoundBox.CornerPoint(1).z,
		MeshDataStruct.BoundBox.CornerPoint(2).x,MeshDataStruct.BoundBox.CornerPoint(2).y,MeshDataStruct.BoundBox.CornerPoint(2).z,
		MeshDataStruct.BoundBox.CornerPoint(3).x,MeshDataStruct.BoundBox.CornerPoint(3).y,MeshDataStruct.BoundBox.CornerPoint(3).z,
		MeshDataStruct.BoundBox.CornerPoint(4).x,MeshDataStruct.BoundBox.CornerPoint(4).y,MeshDataStruct.BoundBox.CornerPoint(4).z,
		MeshDataStruct.BoundBox.CornerPoint(5).x,MeshDataStruct.BoundBox.CornerPoint(5).y,MeshDataStruct.BoundBox.CornerPoint(5).z,
		MeshDataStruct.BoundBox.CornerPoint(6).x,MeshDataStruct.BoundBox.CornerPoint(6).y,MeshDataStruct.BoundBox.CornerPoint(6).z,
		MeshDataStruct.BoundBox.CornerPoint(7).x,MeshDataStruct.BoundBox.CornerPoint(7).y,MeshDataStruct.BoundBox.CornerPoint(7).z
	};
	memcpy(DebugDrawAABB_vertices, vertices, sizeof(float) * 24);

	DebugDrawAABB_indices = new uint[24];
	uint indices[] =
	{
		0,2,
		0,4,
		0,1,
		7,6,
		7,3,
		7,5,
		5,1,
		5,4,
		2,3,
		2,6,
		6,4,
		3,1
	};
	memcpy(DebugDrawAABB_indices, indices, sizeof(uint) * 24);

	//Buffer for vertex
	glGenBuffers(1, (GLuint*) &DebugDrawAABB_id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, DebugDrawAABB_id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, DebugDrawAABB_vertices, GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*) &DebugDrawAABB_id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawAABB_id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 24, DebugDrawAABB_indices, GL_STATIC_DRAW);
}

void ComponentMesh::GenerateOBBDraw()
{
	//Clean if there is another aabb allocated
	if (DebugDrawOBB_vertices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawOBB_id_vertices);
		RELEASE_ARRAY(DebugDrawOBB_vertices);
	}
	if (DebugDrawOBB_indices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawOBB_id_indices);
		RELEASE_ARRAY(DebugDrawOBB_indices);
	}

	//Box math
	DebugDrawOBB_vertices = new float[24];
	float vertices[] =
	{
		MeshDataStruct.BoundOBox.CornerPoint(0).x,MeshDataStruct.BoundOBox.CornerPoint(0).y,MeshDataStruct.BoundOBox.CornerPoint(0).z,
		MeshDataStruct.BoundOBox.CornerPoint(1).x,MeshDataStruct.BoundOBox.CornerPoint(1).y,MeshDataStruct.BoundOBox.CornerPoint(1).z,
		MeshDataStruct.BoundOBox.CornerPoint(2).x,MeshDataStruct.BoundOBox.CornerPoint(2).y,MeshDataStruct.BoundOBox.CornerPoint(2).z,
		MeshDataStruct.BoundOBox.CornerPoint(3).x,MeshDataStruct.BoundOBox.CornerPoint(3).y,MeshDataStruct.BoundOBox.CornerPoint(3).z,
		MeshDataStruct.BoundOBox.CornerPoint(4).x,MeshDataStruct.BoundOBox.CornerPoint(4).y,MeshDataStruct.BoundOBox.CornerPoint(4).z,
		MeshDataStruct.BoundOBox.CornerPoint(5).x,MeshDataStruct.BoundOBox.CornerPoint(5).y,MeshDataStruct.BoundOBox.CornerPoint(5).z,
		MeshDataStruct.BoundOBox.CornerPoint(6).x,MeshDataStruct.BoundOBox.CornerPoint(6).y,MeshDataStruct.BoundOBox.CornerPoint(6).z,
		MeshDataStruct.BoundOBox.CornerPoint(7).x,MeshDataStruct.BoundOBox.CornerPoint(7).y,MeshDataStruct.BoundOBox.CornerPoint(7).z
	};
	memcpy(DebugDrawOBB_vertices, vertices, sizeof(float) * 24);

	DebugDrawOBB_indices = new uint[24];
	uint indices[] =
	{
		0,2,
		0,4,
		0,1,
		7,6,
		7,3,
		7,5,
		5,1,
		5,4,
		2,3,
		2,6,
		6,4,
		3,1
	};
	memcpy(DebugDrawOBB_indices, indices, sizeof(uint) * 24);

	//Buffer for vertex
	glGenBuffers(1, (GLuint*)&DebugDrawOBB_id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, DebugDrawOBB_id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, DebugDrawOBB_vertices, GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*)&DebugDrawOBB_id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawOBB_id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 24, DebugDrawOBB_indices, GL_STATIC_DRAW);
}