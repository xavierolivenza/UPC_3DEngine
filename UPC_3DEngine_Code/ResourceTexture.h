#pragma once

#include "Resource.h"

struct TextureData
{
	uint id_texture = 0; // id in VRAM
	std::string texture_name = "";
	uint texture_w = 0;
	uint texture_h = 0;
	uint texture_d = 0;

	TextureData()
	{

	}

	~TextureData()
	{
		texture_name.clear();
		if ((texture_name != "") && (id_texture > 0))
			glDeleteTextures(1, &id_texture);
	}
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	~ResourceTexture();

	void Save(JSON_Object* conf) const;
	void Load(JSON_Object* conf);

public:
	TextureData TextureDataStruct;

};