#include "Component.h"

Component::Component()
{

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

void Component::SetActive(bool active)
{
	if (Active != active)
	{
		Active = active;
		if (Active) Enable();
		else Disable();
	}
}

ComponentType Component::GetType()
{
	return type;
}