#pragma once

#include "Resource.h"
#include "Glew\include\glew.h"

#include "ComponentMaterial.h" //will be deleted and MeshData defined here

struct MaterialData
{
	uint id_texture = 0; // id in VRAM
	std::string texture_name = "";
	uint texture_w = 0;
	uint texture_h = 0;
	uint texture_d = 0;

	MaterialData()
	{

	}

	~MaterialData()
	{
		Clean();
	}

	void Clean()
	{
		if ((texture_name != "") && (id_texture > 0))
			glDeleteTextures(1, &id_texture);
		texture_name.clear();
	}
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	~ResourceTexture();

	void CleanResource();

	void DrawResourceOptionsAndData();

	void Save(JSON_Object* conf) const;
	void Load(JSON_Object* conf);

public:
	MaterialData TextureDataStruct;

};