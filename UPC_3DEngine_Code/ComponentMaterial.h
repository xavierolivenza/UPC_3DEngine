#pragma once
#include "Component.h"

class ResourceTexture;

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

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent, bool Active);
	~ComponentMaterial();

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

public:
	ResourceTexture* resourceTexture = nullptr;
	MaterialData MaterialDataStruct;
};