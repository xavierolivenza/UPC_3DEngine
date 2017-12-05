#ifndef __ComponentParticleSystem_H__
#define __ComponentParticleSystem_H__

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

private:
	ParticleSystem* PartSystem = nullptr;
};

#endif // __ComponentParticleSystem_H__