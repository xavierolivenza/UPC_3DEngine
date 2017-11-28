#include "ComponentParticleSystem.h"

ComponentParticleSystem::ComponentParticleSystem(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Mesh_Component)
{
	if (Active) Enable();
}

ComponentParticleSystem::~ComponentParticleSystem()
{

}

bool ComponentParticleSystem::Enable()
{
	return true;
}

bool ComponentParticleSystem::PreUpdate(float dt)
{
	return true;
}

bool ComponentParticleSystem::Update(float dt)
{
	return true;
}

bool ComponentParticleSystem::PostUpdate(float dt)
{
	return true;
}

bool ComponentParticleSystem::Disable()
{
	return true;
}

bool ComponentParticleSystem::CleanUp()
{
	return true;
}

void ComponentParticleSystem::DrawComponentImGui()
{

}

void ComponentParticleSystem::SetResource(uint uuid)
{

}

bool ComponentParticleSystem::SaveComponent(JSON_Object* conf) const
{
	return true;
}

bool ComponentParticleSystem::LoadComponent(JSON_Object* conf)
{
	return true;
}