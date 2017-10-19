#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

GameObject::GameObject(const char* name, bool active): name(name), Active(active)
{
	
}

GameObject::~GameObject()
{}

bool GameObject::PreUpdate(float dt)
{
	if (Active)
	{
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->PreUpdate(dt);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			(*item)->PreUpdate(dt);
	}
	return true;
}

bool GameObject::Update(float dt)
{
	if (Active)
	{
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->Update(dt);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			(*item)->Update(dt);
		DrawGameObject();
	}
	return true;
}

bool GameObject::PostUpdate(float dt)
{
	if (Active)
	{
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->PostUpdate(dt);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			(*item)->PostUpdate(dt);
	}
	return true;
}

bool GameObject::CleanUp()
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		(*item)->CleanUp();
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->CleanUp();
	//Release memory here
	//Cleanup loop need improvement iterate all the tree and from leaf nodes iterate backwards to clenaup memory
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

void GameObject::DrawComponentImGui()
{
	for (std::vector<Component*>::iterator item = components.begin(); item != components.cend(); ++item)
		(*item)->DrawComponentImGui();
}

void GameObject::DrawGameObject()
{
	const Component* MeshComponent = FindComponentFirst(ComponentType::Mesh_Component);
	if (MeshComponent != nullptr)
	{
		const Component* MaterialComponent = FindComponentFirst(ComponentType::Material_Component);
		App->renderer3D->DrawComponentMeshMaterial((ComponentMesh*)MeshComponent, (ComponentMaterial*)MaterialComponent);
	}
}

void GameObject::AddChild(GameObject* child)
{
	children.push_back(child);
}

bool GameObject::RemoveChild(GameObject* child)
{
	for (std::vector<GameObject*>::iterator item = children.begin(); item != children.cend(); ++item)
		if ((*item) == child)
		{
			(*item)->CleanUp();
			RELEASE(*item);
			return true;
		}
	return false;
}

ComponentTransform* GameObject::CreateTransformComponent(GameObject* parent, bool active)
{
	ComponentTransform* transform = new ComponentTransform(parent, active);
	components.push_back((Component*)transform);
	return transform;
}

ComponentMesh* GameObject::CreateMeshComponent(GameObject* parent, bool active)
{
	ComponentMesh* mesh = new ComponentMesh(parent, active);
	components.push_back((Component*)mesh);
	return mesh;
}

ComponentMaterial* GameObject::CreateMaterialComponent(GameObject* parent, bool active)
{
	ComponentMaterial* material = new ComponentMaterial(parent, active);
	components.push_back((Component*)material);
	return material;
}

ComponentCamera* GameObject::CreateCameraComponent(GameObject* parent, bool active)
{
	ComponentCamera* camera = new ComponentCamera(parent, active);
	components.push_back((Component*)camera);
	return camera;
}

bool GameObject::RemoveComponent(Component* component)
{
	for (std::vector<Component*>::iterator item = components.begin(); item != components.cend(); ++item)
		if ((*item) == component)
		{
			(*item)->CleanUp();
			RELEASE(*item);
			return true;
		}
	return false;
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