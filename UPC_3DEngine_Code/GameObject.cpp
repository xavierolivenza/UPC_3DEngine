#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(const char* name, bool active): name(name), Active(active)
{
	
}

GameObject::~GameObject()
{}

bool GameObject::PreUpdate(float dt)
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		(*item)->PreUpdate(dt);
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->PreUpdate(dt);
	return true;
}

bool GameObject::Update(float dt)
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		(*item)->Update(dt);
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->Update(dt);
	return true;
}

bool GameObject::PostUpdate(float dt)
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		(*item)->PostUpdate(dt);
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->PostUpdate(dt);
	return true;
}

bool GameObject::CleanUp()
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		(*item)->CleanUp();
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->CleanUp();
	if (children.size() == 0)
	{
		//Release memory here

	}
	return true;
}

void GameObject::SetActive(bool active)
{
	if (Active != active)
	{
		Active = active;
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			(*item)->SetActive(Active);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			(*item)->SetActive(Active);
	}
}

const std::vector<GameObject*>* GameObject::GetChildren() const
{
	return &children;
}

const std::vector<Component*>* GameObject::GetComponents() const
{
	return &components;
}

const Component* GameObject::FindComponentFirst(ComponentType type) const
{
	Component* ret = nullptr;
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		if (type == (*item)->GetType())
		{
			ret = (*item);
			break;
		}
	return ret;
}

const Component* GameObject::FindComponentLast(ComponentType type) const
{
	Component* ret = nullptr;
	for (std::vector<Component*>::const_reverse_iterator item = components.crbegin(); item != components.crend(); ++item)
		if (type == (*item)->GetType())
		{
			ret = (*item);
			break;
		}
	return ret;
}

void GameObject::FindComponentVec(std::vector<Component*>& vec, ComponentType type) const
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		if (type == (*item)->GetType())
			vec.push_back(*item);
}