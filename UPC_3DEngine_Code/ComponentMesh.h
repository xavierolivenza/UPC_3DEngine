#pragma once
#include "Component.h"

struct MeshData
{
	AABB BoundBox;
	int num_faces = 0;
	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_normals = 0; // id in VRAM
	float* normals = nullptr;
	uint id_texture_coords = 0; // id in VRAM
	float* texture_coords = nullptr;
	uint id_colors = 0; // id in VRAM
	float* colors = nullptr;

	MeshData()
	{

	}

	~MeshData()
	{
		BoundBox.SetNegativeInfinity();
		if (vertices != nullptr)
		{
			glDeleteBuffers(1, &id_vertices);
			RELEASE_ARRAY(vertices);
		}
		if (indices != nullptr)
		{
			glDeleteBuffers(1, &id_indices);
			RELEASE_ARRAY(indices);
		}
		if (normals != nullptr)
		{
			glDeleteBuffers(1, &id_normals);
			RELEASE_ARRAY(normals);
		}
		if (texture_coords != nullptr)
		{
			glDeleteBuffers(1, &id_texture_coords);
			RELEASE_ARRAY(texture_coords);
		}
		if (colors != nullptr)
		{
			glDeleteBuffers(1, &id_colors);
			RELEASE_ARRAY(colors);
		}
	}
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(bool Active);
	~ComponentMesh();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

public:
	MeshData MeshDataStruct;

};