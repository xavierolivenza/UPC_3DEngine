#pragma once
#include "Component.h"

class ResourceMesh;

struct MeshData
{
	Sphere BoundSphere;
	AABB BoundBox;
	OBB BoundOBox;
	uint num_faces = 0;
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

	std::string Mesh_name;
	std::string Asociated_texture_name;
	std::string Mesh_File;

	MeshData()
	{

	}

	~MeshData()
	{
		BoundSphere.SetNegativeInfinity();
		BoundBox.SetNegativeInfinity();
		BoundOBox.SetNegativeInfinity();
		if (vertices != nullptr)
		{
			if (id_vertices > 0)
				glDeleteBuffers(1, &id_vertices);
			RELEASE_ARRAY(vertices);
		}
		if (indices != nullptr)
		{
			if (id_indices > 0)
				glDeleteBuffers(1, &id_indices);
			RELEASE_ARRAY(indices);
		}
		if (normals != nullptr)
		{
			if (id_normals > 0)
				glDeleteBuffers(1, &id_normals);
			RELEASE_ARRAY(normals);
		}
		if (texture_coords != nullptr)
		{
			if (id_texture_coords > 0)
				glDeleteBuffers(1, &id_texture_coords);
			RELEASE_ARRAY(texture_coords);
		}
		if (colors != nullptr)
		{
			if (id_colors > 0)
				glDeleteBuffers(1, &id_colors);
			RELEASE_ARRAY(colors);
		}
	}
};

class ComponentMesh : public Component
{
	friend class OctreeNode;

public:
	ComponentMesh(GameObject* parent, bool Active);
	~ComponentMesh();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

	void SetResource(uint uuid);

	bool SaveComponent(JSON_Object* conf) const;
	bool LoadComponent(JSON_Object* conf);

	void GetTransformedAABB(AABB& TransformedBox) const;

public:
	ResourceMesh* resourceMesh = nullptr;

private:
	bool DebugDrawAABB = false;
	bool DebugDrawOBB = false;
};