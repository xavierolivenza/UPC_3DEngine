#pragma once

#include "Resource.h"
#include "Glew\include\glew.h"

struct SimpleMeshData
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

	SimpleMeshData()
	{

	}

	~SimpleMeshData()
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

class ResourceMesh : public Resource
{
public:
	ResourceMesh();
	~ResourceMesh();

	void Save(JSON_Object* conf) const;
	void Load(JSON_Object* conf);

public:
	SimpleMeshData SimpleMeshDataStruct;

};