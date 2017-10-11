#include "GameObject.h"

GameObject::GameObject()
{
	
}

GameObject::~GameObject()
{}

bool GameObject::PreUpdate(float dt)
{
	return true;
}

bool GameObject::Update(float dt)
{
	return true;
}

bool GameObject::PostUpdate(float dt)
{
	return true;
}

bool GameObject::CleanUp()
{
	return true;
}

void GameObject::SetActive(bool active)
{
	if (Active != active)
		Active = active;
}