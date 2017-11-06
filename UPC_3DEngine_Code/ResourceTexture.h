#pragma once

#include "Resource.h"

//By now commented, because this will collide with component material MaterialData
//struct MaterialData;

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	~ResourceTexture();

	void Save(JSON_Object* conf) const;
	void Load(JSON_Object* conf);

public:
	//MaterialData DataMaterial;

};
/*
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
		texture_name.clear();
		if ((texture_name != "") && (id_texture > 0))
			glDeleteTextures(1, &id_texture);
	}
};
*/