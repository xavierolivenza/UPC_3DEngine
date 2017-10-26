#include "Component.h"

Component::Component(GameObject* parent, bool Active, uint replica_limit, ComponentType type) : parent(parent), Active(Active), replica_limit(replica_limit), type(type)
{
	LCG UUIDGen;
	UUID = UUIDGen.Int();
}

Component::~Component()
{}

bool Component::Enable()
{
	return true;
}

bool Component::PreUpdate(float dt)
{
	return true;
}

bool Component::Update(float dt)
{
	return true;
}

bool Component::PostUpdate(float dt)
{
	return true;
}

bool Component::Disable()
{
	return true;
}

bool Component::CleanUp()
{
	return true;
}

void Component::DrawComponentImGui()
{

}

void Component::SetActive(bool active)
{
	if (Active != active)
	{
		Active = active;
		if (Active) Enable();
		else Disable();
	}
}

bool Component::IsActive() const
{
	return Active;
}

ComponentType Component::GetType() const
{
	return type;
}