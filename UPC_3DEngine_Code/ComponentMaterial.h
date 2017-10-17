#pragma once
#include "Component.h"

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
		if (texture_name != "")
			glDeleteTextures(1, &id_texture);
	}
};

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(bool Active);
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