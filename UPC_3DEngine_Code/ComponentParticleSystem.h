#pragma once
#include "Component.h"

//class ResourceMesh;
class ParticleSystem;

class ComponentParticleSystem : public Component
{
public:
	ComponentParticleSystem(GameObject* parent, bool Active);
	~ComponentParticleSystem();

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

//public:
//	ResourceMesh* resourceMesh = nullptr;

private:
	ParticleSystem* PartSystem = nullptr;
};