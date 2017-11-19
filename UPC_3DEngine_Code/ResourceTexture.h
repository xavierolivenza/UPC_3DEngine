#pragma once

#include "Resource.h"
#include "Glew\include\glew.h"

#include "DevIL\include\il.h" //Used for initialize variables

struct MaterialData
{
	uint id_texture = 0; // id in VRAM
	std::string texture_name = "";
	std::string texture_name_UI = "";
	uint texture_w = 0;
	uint texture_h = 0;
	uint texture_d = 0;

	//Loading varibles
	GLint WrappingMethod = GL_CLAMP;
	GLint InterpolationMethod = GL_LINEAR;
	//Saving variables
	GLint CompressingMethod = IL_DXT5;

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
private:
	int WrappingMethodPick = 0;
	int InterpolationMethodPick = 1;
	int CompressingMethodPick = 4;
};