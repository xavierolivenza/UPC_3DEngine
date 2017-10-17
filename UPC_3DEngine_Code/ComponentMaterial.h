#pragma once
#include "Component.h"

struct MaterialData
{
	uint id_colors = 0; // id in VRAM
	float* colors = nullptr;
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
		if (colors != nullptr)
		{
			glDeleteBuffers(1, &id_colors);
			RELEASE_ARRAY(colors);
		}
		texture_name.clear();
		if (texture_name != "")
			glDeleteTextures(1, &id_texture);
	}
};

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

public:
	MaterialData MaterialDataStruct;
};